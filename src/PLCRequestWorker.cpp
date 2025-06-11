#include "PLCRequestWorker.hpp"
#include "Utilities.hpp"

// シングルトンインスタンス取得
PLCRequestWorker& PLCRequestWorker::getInstance(PLCConnectionClient& plcclient) {
    static PLCRequestWorker instance;
    instance.pLCConnectionClient_ = &plcclient;
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
    while (!gShouldExit) {
        // PLC接続フラグをtrueにする
        gPLCconnectFlag = true; 

        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }

        // キューからTransactionDataを取り出す
        PLCTransactionData req;
        {
            unique_lock<mutex> lock(gRequestQueueMutex);
            if (gRequestQueue.empty()) {
                // cv.wait(lock);
                continue;
            }
            req = gRequestQueue.front();
            gRequestQueue.pop_front();
        }
        Logger::getInstance().Debug("キューから取り出しました。\n 【MCプロトコル】" + Utilities::convertVectorBytesToHexString(req.protocolbuf));

        // LumpDataを取得
        DataLump* dataLump = getDataLump(req);

        // PLCへリクエストを送信        
        Logger::getInstance().Debug("PLCにリクエストを送ります。");

        // 送信リクエストをPLCへ送信
        int sendLen = 0;
        int sendTryTimes = 0;
        bool resetFlag = false;
        
        while(pLCConnectionClient_->sendRequest(req.protocolbuf.data(), req.protocolbuf.size(), sendLen) < 0 && !gShouldExit)
        {
            Logger::getInstance().Error("PLCへの送信が失敗しました。");
            if (sendTryTimes > 2)
            {
                Logger::getInstance().Error("試行規定回数に達しました。ソケットを閉じて再接続します。");
                
                // 5回送信しても失敗した場合、ソケットを閉じて再接続する。
                pLCConnectionClient_->Close();
                pLCConnectionClient_->makeSocket();
                dataLump->allClear(); // DataLumpをクリア
                Logger::getInstance().Error("スケジューラのキュープッシュを停止します。");
                gPLCconnectFlag = false; // PLC接続フラグをfalseにする
                // キューをロックして空にする。
                {
                    unique_lock<mutex> lock(gRequestQueueMutex);
                    while(!gClearQueueFlag){}
                    gRequestQueue.clear(); 
                    Logger::getInstance().Error("キューを空にしました。");
                }
                while(pLCConnectionClient_->Connect() < 0 && !gShouldExit)
                {
                    Logger::getInstance().Error("PLC接続再試行中...");
                    this_thread::sleep_for(chrono::seconds(1));
                }
                // 終了信号を受け取ったときの処理
                if (gShouldExit) break;

                Logger::getInstance().Error("PLC接続に成功しました。");
                resetFlag = true;
                sendTryTimes = 0; // 再接続後、送信試行回数をリセット
                break;
            }
            // 送信エラーが発生した場合、再送信を試みる。
            Logger::getInstance().Error("再送信します。");
            this_thread::sleep_for(chrono::milliseconds(50));
            sendTryTimes++;
        }
        
        // 終了信号を受け取ったときの処理
        if (gShouldExit) break;

        // リセットフラグが経っていたらループを最初から
        if (resetFlag) continue;

        Logger::getInstance().Debug("送信データをPLCへ送信しました。");
        resetFlag = false; // リセットフラグをfalseにする
        gPLCconnectFlag = true; // PLC接続フラグをtrueにする
        gClearQueueFlag = false; // キューを空にするフラグをfalseにする
        


        // PLCからのレスポンス受信
        char text[recvBufferSize];
        int recvLen = 0;
        int recvTryTimes = 0;
        while(pLCConnectionClient_->recvResponse(text, recvBufferSize, recvLen) < 0 && !gShouldExit)
        {
            Logger::getInstance().Error("PLCからのデータ受信が失敗しました。");
            if (recvTryTimes > 2)
            {
                Logger::getInstance().Error("試行規定回数に達しました。ソケットを閉じて再接続します。");
                
                // 5回送信しても失敗した場合、ソケットを閉じて再接続する。
                pLCConnectionClient_->Close();
                pLCConnectionClient_->makeSocket();
                dataLump->allClear(); // DataLumpをクリア
                Logger::getInstance().Error("スケジューラのキュープッシュを停止します。");
                gPLCconnectFlag = false; // PLC接続フラグをfalseにする
                // キューをロックして空にする。
                {
                    unique_lock<mutex> lock(gRequestQueueMutex);
                    while(!gClearQueueFlag){}
                    gRequestQueue.clear(); 
                    Logger::getInstance().Error("キューを空にしました。");
                }
                while(pLCConnectionClient_->Connect() < 0 && !gShouldExit)
                {
                    Logger::getInstance().Error("PLC接続再試行中...");
                    this_thread::sleep_for(chrono::seconds(1));
                }

                // 終了信号を受け取ったときの処理
                if (gShouldExit) break;

                Logger::getInstance().Error("PLC接続に成功しました。");
                resetFlag = true;
                recvTryTimes = 0; // 再接続後、送信試行回数をリセット
                break;
            }
            // 受信エラーが発生した場合、再受信を試みる。
            Logger::getInstance().Error("再受信します。");
            this_thread::sleep_for(chrono::milliseconds(50));
            recvTryTimes++;
        }

        // 終了信号を受け取ったときの処理
        if (gShouldExit) break;

        // リセットフラグが立っていたらループを最初から
        if (resetFlag) continue;

        Logger::getInstance().Debug("受信データを受け取りました。");
        resetFlag = false; // リセットフラグをfalseにする
        gPLCconnectFlag = true; // PLC接続フラグをtrueにする
        gClearQueueFlag = false; // キューを空にするフラグをfalseにする

        // req.receiptTime = Logger::getInstance().timestamp;

        // 送信データ作成
        Logger::getInstance().Debug("送信データを作成します");
        vector<map<string,string>> sendData = MCprotocolRecvDataManager::getInstance().convertResponseDataToSendData(text, recvLen, req);
        Logger::getInstance().Debug("送信データ: " + Utilities::convertVectorMapToString(sendData));

        // 受信データを確認し、sensorの準備状態を変更する。
        dataLump = getReadySensor(dataLump, sendData);

        if (dataLump != nullptr && dataLump->isSendReady) {
            // 送信データをPLCへ送信
            Logger::getInstance().Debug("データをサーバへ送信します。");
            vector<map<string,string>> sendDatacp = dataLump->sendData;
            gSendDataMap.push_back(sendDatacp);
            dataLump->allClear();
        }
    }
}

DataLump* PLCRequestWorker::getDataLump(PLCTransactionData& req)
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
    return dataLump;
}


DataLump* PLCRequestWorker::getReadySensor(DataLump* dataLump, const vector<map<string,string>> sendDataNow)
{
    // 送信間隔が同じものがなければ、何もしない
    if (dataLump == nullptr)
    {
        return nullptr;
    }

    // DataLumpにsendDataを格納
    Utilities::appendVectorElements(dataLump->sendData, sendDataNow);

    // sendDtataの中から、sensorIDを探す
    for (auto& row : sendDataNow)
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
            Logger::getInstance().Debug("Sensor ID: " + sensorID + ", Ready: " + to_string(ready));
        }
    }


    // LumpFullを確認
    dataLump->isLumpFull();

    Logger::getInstance().Debug("LumpFull: " + to_string(dataLump->isSendReady));

    return dataLump;
}