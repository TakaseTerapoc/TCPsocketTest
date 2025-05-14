#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "globals.hpp"
#include "../external/fmt/format.h"
#include "ServerConnectionClient.hpp"

/**
 * @brief SendDataが格納されているリストを監視し、サーバーへリクエストを送るクラスです。
 */
class ServerRequestWorker
{
    public:
        // シングルトン取得
        static ServerRequestWorker& getInstance(ServerConnectionClient& serverClient);

        // ワーカー開始
        void start();

        // ワーカーjoin
        void join();

        // ワーカー停止
        void stop();

    private:
        // コンストラクタ
        ServerRequestWorker() = default;

        // デストラクタ
        ~ServerRequestWorker() = default; 
        ServerRequestWorker(const ServerRequestWorker&) = delete;
        ServerRequestWorker& operator=(const ServerRequestWorker&) = delete;

        // キューから出して、TCPリクエストを依頼する。
        void run();  

        std::thread      thread_;                   // 実行スレッド
        bool             running_{false};           // 実行中フラグ
        std::mutex       mutex_;                    // running_ の排他制御
        ServerConnectionClient serverConnectionClient_;   // ServerConnectionClientのインスタンス
};