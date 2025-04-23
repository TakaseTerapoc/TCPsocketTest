#include "PLCRequestScheduler.hpp"

// シングルトンインスタンス取得
RequestScheduler& RequestScheduler::getInstance() {
    static RequestScheduler instance;
    return instance;
}

// スケジューラ開始
void RequestScheduler::start() {
    std::lock_guard<std::mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&RequestScheduler::run, this);
}

// スケジューラ停止
void RequestScheduler::stop() {
    {
        std::lock_guard<std::mutex> lg(mutex_);
        running_ = false;
    }
    if (thread_.joinable()) {
        thread_.join();
    }
}

// スケジュール追加
void RequestScheduler::addSchedule(int address, int count, int intervalMs) {
    std::lock_guard<std::mutex> lg(mutex_);
    schedules_.push_back({
        address,
        count,
        intervalMs,
        std::chrono::steady_clock::now()
    });
}

// ワーカーループ: 各周期でリクエストをキューに追加
void RequestScheduler::run() {
    while (true) {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            if (!running_) break;
        }

        auto now = std::chrono::steady_clock::now();
        for (auto& s : schedules_) {
            if (now >= s.nextTime) {
                {
                    std::lock_guard<std::mutex> ql(PLC::queueMutex);
                    PLC::requestQueue.push({s.address, s.count});
                }
                PLC::cv.notify_one();
                s.nextTime = now + std::chrono::milliseconds(s.intervalMs);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
