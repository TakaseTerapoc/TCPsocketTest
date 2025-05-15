#include "PLCRequestWorker.hpp"

// シングルトンインスタンス取得
PLCRequestWorker& PLCRequestWorker::getInstance(PLCConnectionClient& plcclient) {
    static PLCRequestWorker instance;
    instance.pLCConnectionClient_ = plcclient;
    return instance;
}

// ワーカー開始
void PLCRequestWorker::start() {
    std::lock_guard<std::mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&PLCRequestWorker::run, this);
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
        std::lock_guard<std::mutex> lg(mutex_);
        running_ = false;
    }
    join();
}

// キューから出し、PLCへのTCPリクエストを依頼する。
void PLCRequestWorker::run() {

    // まとめて送信するデータを格納するリスト
    std::vector<DataLumpBase*> dataLumps;
    DataLumpTest* dataLumpTest = new DataLumpTest();

    dataLumps.push_back(dataLumpTest);

    while (true) {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            if (!running_) break;
        }
        PLCRequestResponseData req;
        {
            std::unique_lock<std::mutex> lock(gRequestQueueMutex);
            if (gRequestQueue.empty()) {
                // cv.wait(lock);
                continue;
            }
            req = gRequestQueue.front();
            gRequestQueue.pop();
        }

        // 確認やつ
        auto now = std::chrono::steady_clock::now();
        auto dur = now.time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        Logger::getInstance().Info("キューから取り出しました。" + req.serialNumber + "【時間】" + std::to_string(ms));

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
        // std::vector<std::vector<std::string>> sendData = MCprotocolManager::convertResponseDataToSendData(text, len, req);
        std::vector<std::map<std::string,std::string>> sendData = MCprotocolManager::convertResponseDataToSendData2(text, len, req);
        std::string sendDatastr;
        for (size_t i = 0; i < sendData.size(); ++i) {
            for (const auto& [key, value] : sendData[i]) {
                    sendDatastr += key + ": " + value + " | ";
                }
            sendDatastr += ", ";
        }
        Logger::getInstance().Info("送信データ: " + sendDatastr);
            
        std::string test; // 結果を格納する変数

        for (auto& row : sendData) {
            auto catIt = row.find("categoryID");
            auto sensorIt = row.find("sensorID");

            if (catIt != row.end() && sensorIt != row.end()) {
                if (catIt->second == "CATE100" ){
                    if(sensorIt->second == "AAAA1")
                    {
                        dataLumpTest->setAAAA1(row);
                    }
                    else if(sensorIt->second == "AAAA2")
                    {
                        dataLumpTest->setAAAA2(row);
                    }
                    else if(sensorIt->second == "AAAA4")
                    {
                        dataLumpTest->setAAAA4(row);
                    }
                    else if(sensorIt->second == "AAAA7")
                    {
                        dataLumpTest->setAAAA7(row);
                    }
                    else if(sensorIt->second == "AAAA10")
                    {
                        dataLumpTest->setAAAA10(row);
                    }
                    else if(sensorIt->second == "AAAA99")
                    {
                        dataLumpTest->setAAAA99(row);
                    }
                }
                else{
                    Logger::getInstance().Error("categoryIDがありません。");
                }
            }
        }

        if (dataLumpTest->isLumpFull()) {
            // 送信データをPLCへ送信
            Logger::getInstance().Info("データをサーバへ送信します。");
            dataLumpTest->setLumpFull(false);
        }


        // for (int i = 0; i < sendData.size(); i++)
        // {
        //     for (int j = 0; j < sendData[i].size(); j++)
        //     {
        //         Logger::getInstance().Info("sendData[" + std::to_string(i) + "][" + std::to_string(j) + "]" + sendData[i][j]);
        //     }
        // }
        // sendData確認

        // gSendDataに格納
        // {
        //     std::lock_guard<std::mutex> lock(gSendDataMutex);
        //     for (int i = 0; i < sendData.size(); i++)
        //     {
        //         gSendData.push_back(sendData[i]);
        //     }
        // }
    }
}