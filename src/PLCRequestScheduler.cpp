#include "PLCRequestScheduler.hpp"

// シングルトンインスタンス取得
PLCRequestScheduler& PLCRequestScheduler::getInstance() {
    static PLCRequestScheduler instance;
    return instance;
}

// スケジューラ開始
void PLCRequestScheduler::start() {
    lock_guard<mutex> lg(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = thread(&PLCRequestScheduler::run, this);
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
        lock_guard<mutex> lg(mutex_);
        running_ = false;
    }
    join();
}

// 各リクエストの周期でリクエストをキューに追加
void PLCRequestScheduler::run() {
    while (true) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }
        if (!gPLconnectFlag)
        {
            this_thread::sleep_for(chrono::milliseconds(50));
            continue;
        }

        auto now = chrono::steady_clock::now();
        for (auto& plcr : gRData) {
            if (now >= plcr.nextTime) {
                {
                    lock_guard<mutex> ql(gRequestQueueMutex);
                    gRequestQueue.push(plcr);
                }
                // PLC::cv.notify_one();
                plcr.nextTime = now + chrono::milliseconds(plcr.sendIntervalMs);
            }
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}
