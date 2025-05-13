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
        pLCConnectionClient_.SendRequest(req.protocolbuf.data(), 12);
        Logger::getInstance().Info("RecvResponseを動かします。");

        // レスポンス受信
        char text[256];
        int len = pLCConnectionClient_.RecvResponse(text);
        req.receiptTime = Logger::getInstance().timestamp;
        if (len > 0)
        {
            Logger::getInstance().Info("【デバイスコード】" + req.deviceCode );
        }
        Logger::getInstance().Info("送信データを作成します");
        std::vector<std::vector<std::string>> sendData = MCprotocolManager::convertSendData(text, len, req);
        
        // sendData確認
        for (int i = 0; i < sendData.size(); i++)
        {
            for (int j = 0; j < sendData[i].size(); j++)
            {
                Logger::getInstance().Info("sendData[" + std::to_string(i) + "][" + std::to_string(j) + "]" + sendData[i][j]);
            }
        }

    }
}