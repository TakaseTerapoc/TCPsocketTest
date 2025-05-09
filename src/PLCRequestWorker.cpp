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
        PLCRequestData req;
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
        std::cout << "キューから取り出しました。" << req.serialNumber << "->時間"<< ms << std::endl;

        // TCPリクエスト
        pLCConnectionClient_.SendRequest(req.protocolbuf.data(), req.protocolbuf.size());
        std::cout << "RecvResponseを動かします。"<< std::endl; 
        char text[256];

        // レスポンス受信
        if (pLCConnectionClient_.RecvResponse(text) > 0)
        {
            Logger::getInstance().Sensor(text);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}