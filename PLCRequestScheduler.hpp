#ifndef PLCREQUESTSCHEDULER_HPP
#define PLCREQUESTSCHEDULER_HPP

#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "PLCRequestData.hpp"
#include "plc_globals.h"  // PlcRequest, PLC::requestQueue, PLC::queueMutex, PLC::cv を定義

class PLCRequestScheduler {
public:
    // シングルトン取得
    static PLCRequestScheduler& getInstance();

    // スケジューラ開始
    void start();

    // スケジューラ停止（終了を待つ）
    void stop();

    // スケジュールを追加
    void addSchedule(int address, int count, int intervalMs);

private:
    RequestScheduler();
    ~RequestScheduler();
    RequestScheduler(const RequestScheduler&) = delete;
    RequestScheduler& operator=(const RequestScheduler&) = delete;

    struct Schedule {
        int address;  // 読み取り開始アドレス
        int count;    // 読み取る点数
        int intervalMs;                   // リクエスト周期（ms）
        std::chrono::steady_clock::time_point nextTime;  // 次回実行時刻
    };

    void run();  // ワーカーループ

    std::vector<Schedule> schedules_;  // スケジュールリスト
    std::thread      thread_;         // 実行スレッド
    bool             running_{false}; // 実行中フラグ
    std::mutex       mutex_;          // schedules_ と running_ の排他制御
};

#endif