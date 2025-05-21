#ifndef PLCREQUESTSCHEDULER_HPP
#define PLCREQUESTSCHEDULER_HPP

#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "PLCRequestResponseData.hpp"
#include "globals.hpp"

using namespace std;

/**
 * @brief PLCRequestDataを格納されているインターバルのタイミングでキューに格納するクラスです。
 */
class PLCRequestScheduler {
public:
    // シングルトン取得
    static PLCRequestScheduler& getInstance();

    // スケジューラ開始
    void start();

    // スケジューラjoin
    void join();

    // スケジューラ停止
    void stop();

private:
    // コンストラクタ
    PLCRequestScheduler() = default;

    // デストラクタ
    ~PLCRequestScheduler() = default;

    // コピー禁止 
    PLCRequestScheduler(const PLCRequestScheduler&) = delete;
    PLCRequestScheduler& operator=(const PLCRequestScheduler&) = delete;

    void run();  // キューに送る

    std::thread      thread_;         // 実行スレッド
    bool             running_{false}; // 実行中フラグ
    std::mutex       mutex_;          // running_ の排他制御
};

#endif