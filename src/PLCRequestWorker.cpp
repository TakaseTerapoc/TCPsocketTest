#include "PLCRequestWorker.hpp"

// シングルトンインスタンス取得
PLCRequestWorker& PLCRequestWorker::getInstance(PLCConnectionClient& plcclient) {
    static PLCRequestWorker instance;
    instance.pLCConnectionClient_ = plcclient;
    return instance;
}

// ワーカー開始
void PLCRequestWorker::start() {
    lock_guard<mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = thread(&PLCRequestWorker::run, this);
}

// ワーカjoin
void PLCRequestWorker::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

// ワーカー停止
void PLCRequestWorker::stop() {
    {
        lock_guard<mutex> lg(mutex_);
        running_ = false;
    }
    join();
}

// キューから出し、PLCへのTCPリクエストを依頼する。
void PLCRequestWorker::run() {
    while (true) {
        gPLconnectFlag = true; // PLC接続フラグをtrueにする
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }
        PLCTransactionData req;
        {
            unique_lock<mutex> lock(gRequestQueueMutex);
            if (gRequestQueue.empty()) {
                // cv.wait(lock);
                continue;
            }
            req = gRequestQueue.front();
            gRequestQueue.pop();
        }
        Logger::getInstance().Info("キューから取り出しました。");

        // TCPリクエスト        
        Logger::getInstance().Info("PLCにリクエストを送ります。");
        for (int i = 0; i < req.protocolbuf.size(); ++i)
        {
            printf("%02X ", req.protocolbuf[i]);
        }
        printf("\n");
        int sendLen = 0;
        int sendTryTimes = 0;
        while(pLCConnectionClient_.sendRequest(req.protocolbuf.data(), req.protocolbuf.size(), sendLen) <= 0)
        {
            Logger::getInstance().Error("PLCへの送信が失敗しました。");
            if (sendTryTimes > 4)
            {
                Logger::getInstance().Error("試行規定回数に達しました。ソケットを閉じて再接続します。");
                
                // 3回送信しても失敗した場合、ソケットを閉じて再接続する。
                pLCConnectionClient_.close();
                pLCConnectionClient_.makeSocket();
                while(pLCConnectionClient_.Connect() < 0)
                {
                    Logger::getInstance().Error("PLC接続再試行中...");
                    this_thread::sleep_for(chrono::seconds(1));
                }
                sendTryTimes = 0; // 再接続後、送信試行回数をリセット
            }
            // 送信エラーが発生した場合、再送信を試みる。
            Logger::getInstance().Error("再送信します。");
            this_thread::sleep_for(chrono::milliseconds(50));
            sendTryTimes++;
        }
        Logger::getInstance().Info("RecvResponseを動かします。");

        // レスポンス受信
        char text[256];
        int recvLen = 0;
        int recvTryTimes = 0;
        bool resetFlag = false;
        while(pLCConnectionClient_.recvResponse(text, recvLen) <= 0)
        {
            Logger::getInstance().Error("受信が失敗しました。");
            if (recvTryTimes > 4)
            {
                Logger::getInstance().Error("試行規定回数に達しました。ソケットを閉じて再接続します。");
                
                // 5回送信しても失敗した場合、ソケットを閉じて再接続する。
                pLCConnectionClient_.close();
                pLCConnectionClient_.makeSocket();
                gPLconnectFlag = false; // PLC接続フラグをfalseにする
                // キューをロックして空にする。
                {
                    unique_lock<mutex> lock(gRequestQueueMutex);
                    gRequestQueue.clear(); 
                }
                while(pLCConnectionClient_.Connect() < 0)
                {
                    Logger::getInstance().Error("PLC接続再試行中...");
                    this_thread::sleep_for(chrono::seconds(1));
                }
                resetFlag = true;
                recvTryTimes = 0; // 再接続後、送信試行回数をリセット
                break;
            }
            if (resetFlag)
            {
                break;
            }
            // 送信エラーが発生した場合、再送信を試みる。
            Logger::getInstance().Error("再受信します。");
            this_thread::sleep_for(chrono::milliseconds(50));
            recvTryTimes++;
        }
        if (resetFlag)
        {
            continue;
        }
        req.receiptTime = Logger::getInstance().timestamp;

        // 送信データ作成
        Logger::getInstance().Info("送信データを作成します");
        vector<map<string,string>> sendData = MCprotocolManager::convertResponseDataToSendData2(text, recvLen, req);
        string sendDatastr;
        for (size_t i = 0; i < sendData.size(); ++i) {
            for (const auto& [key, value] : sendData[i]) {
                    sendDatastr += key + ": " + value + " | ";
                }
            sendDatastr += ", ";
        }
        Logger::getInstance().Info("送信データ: " + sendDatastr);

        // 受信データを確認し、sensorの準備状態を変更する。
        DataLump* dataLump = getReadySensor(req, sendData);

        if (dataLump != nullptr && dataLump->isSendReady) {
            // 送信データをPLCへ送信
            Logger::getInstance().Info("データをサーバへ送信します。");
            vector<map<string,string>> sendDatacp = dataLump->sendData;
            gSendDataMap.push_back(sendDatacp);
            dataLump->allClear();
        }
    }
}

    DataLump* PLCRequestWorker::getReadySensor(const PLCTransactionData& req, const vector<map<string,string>>& sendData)
    {
        DataLump* dataLump = nullptr;

        // DataLumpの中から、送信間隔が同じものを探す
        for (auto& row : gDataLump)
        {
            if (row.sendIntervalMs == req.sendIntervalMs)
            {
                dataLump = &row;
                break;
            }
        }

        // 送信間隔が同じものがなければ、何もしない
        if (dataLump == nullptr)
        {
            return nullptr;
        }

        // DataLumpにsendDataを格納
        dataLump->sendData.insert(dataLump->sendData.end(), sendData.begin(), sendData.end());


        // sendDtataの中から、sensorIDを探す
        for (auto& row : sendData)
        {
            auto it = row.find("sensorID");
            if (it != row.end())
            {
                // sensorIDを取得
                string sensorID = it->second;
                // sensorIDがtempDataLumpの中にあれば、trueにする
                for (auto& status : dataLump->sensorReadyStatus)
                {
                    auto it2 = status.find(sensorID);
                    if (it2 != status.end())
                    {
                        it2->second = true;
                    }
                }
            }
        }

        // sensorReadyStatusの状態を出力
        for (const auto& status : dataLump->sensorReadyStatus)
        {
            for (const auto& [sensorID, ready] : status)
            {
                Logger::getInstance().Info("Sensor ID: " + sensorID + ", Ready: " + to_string(ready));
            }
        }


        // LumpFullを確認
        dataLump->isLumpFull();

        Logger::getInstance().Info("LumpFull: " + to_string(dataLump->isSendReady));

        return dataLump;
    }