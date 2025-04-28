#include "PLCRequestScheduler.hpp"

// シングルトンインスタンス取得
PLCRequestScheduler& PLCRequestScheduler::getInstance() {
    static PLCRequestScheduler instance;
    return instance;
}

// スケジューラ開始
void PLCRequestScheduler::start() {
    std::lock_guard<std::mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&PLCRequestScheduler::run, this);
}

// スケジューラjoin
void PLCRequestScheduler::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

// スケジューラ停止
void PLCRequestScheduler::stop() {
    {
        std::lock_guard<std::mutex> lg(mutex_);
        running_ = false;
    }
    join();
}

// 各リクエストの周期でリクエストをキューに追加
void PLCRequestScheduler::run() {
    while (true) {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            if (!running_) break;
        }

        auto now = std::chrono::steady_clock::now();
        for (auto& plcr : gRData) {
            if (now >= plcr.nextTime) {
                {
                    std::lock_guard<std::mutex> ql(gRequestQueueMutex);
                    gRequestQueue.push(plcr);
                }
                // PLC::cv.notify_one();
                plcr.nextTime = now + std::chrono::milliseconds(plcr.transmissionIntervalMs);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
