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

        // 確認やつ
        auto now = chrono::steady_clock::now();
        auto dur = now.time_since_epoch();
        auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();
        Logger::getInstance().Debug("キューから取り出しました。" + req.serialNumber + "【時間】" + to_string(ms));

        // TCPリクエスト 
        pLCConnectionClient_.SendRequest(req.protocolbuf.data(), req.protocolbuf.size());
        Logger::getInstance().Info("RecvResponseを動かします。");

        // レスポンス受信
        char text[256];
        int len = pLCConnectionClient_.RecvResponse(text);
        req.receiptTime = Logger::getInstance().timestamp;
        if (len > 0)
        {
            Logger::getInstance().Info("【デバイスコード】" + req.deviceCode );
        }
        else
        {
            Logger::getInstance().Error("レスポンス受信に失敗しました。");
            continue;
        }

        // 送信データ作成
        Logger::getInstance().Info("送信データを作成します");
        // vector<vector<string>> sendData = MCprotocolManager::convertResponseDataToSendData(text, len, req);
        vector<map<string,string>> sendData = MCprotocolManager::convertResponseDataToSendData2(text, len, req);
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

        if (dataLump != nullptr && dataLump->lumpFull) {
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

        Logger::getInstance().Info("LumpFull: " + to_string(dataLump->lumpFull));

        return dataLump;
    }