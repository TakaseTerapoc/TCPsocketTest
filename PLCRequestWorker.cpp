#include "PLCRequestWorker.hpp"

// シングルトンインスタンス取得
PLCRequestWorker& PLCRequestWorker::getInstance() {
    static PLCRequestWorker instance;
    return instance;
}

// ワーカー開始
void PLCRequestWorker::start() {
    std::lock_guard<std::mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&PLCRequestWorker::run, this);
}

// ワーカー停止
void PLCRequestWorker::stop() {
    {
        std::lock_guard<std::mutex> lg(mutex_);
        running_ = false;
    }
    if (thread_.joinable()) {
        thread_.join();
    }
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
            std::unique_lock<std::mutex> lock(requestQueueMutex);
            if (requestQueue.empty()) {
                // cv.wait(lock);
                continue;
            }
            req = requestQueue.front();
            requestQueue.pop();
        }
        auto now = std::chrono::steady_clock::now();
        auto dur = now.time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        std::cout << "キューから取り出しました。" << req.serialNumber << "->時間"<< ms << std::endl;


    }
}