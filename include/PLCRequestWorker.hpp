#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "globals.hpp"
#include "PLCRequestResponseData.hpp"
#include "MCprotocolManager.hpp"
#include "../external/fmt/format.h"
#include "DataLump.hpp"
#include "DataLumpTest.hpp"


/**
 * @brief PLCRequestDataが格納されているキューを監視し、２００ms秒ごとにTCPへリクエストを送るクラスです。
 */
class PLCRequestWorker
{
    public:
        // シングルトン取得
        static PLCRequestWorker& getInstance(PLCConnectionClient& plcclient);

        // ワーカー開始
        void start();

        // ワーカーjoin
        void join();

        // ワーカー停止
        void stop();

    private:
        // コンストラクタ
        PLCRequestWorker() = default;

        // デストラクタ
        ~PLCRequestWorker() = default; 
        PLCRequestWorker(const PLCRequestWorker&) = delete;
        PLCRequestWorker& operator=(const PLCRequestWorker&) = delete;

        // キューから出して、TCPリクエストを依頼する。
        void run();

        // sensorReadyStatusにsensorIDの状態を格納する関数
        DataLump* getReadySensor(const PLCRequestResponseData& req, const vector<map<string,string>>& sendData);

        std::thread      thread_;                   // 実行スレッド
        
        bool             running_{false};           // 実行中フラグ

        std::mutex       mutex_;                    // running_ の排他制御

        PLCConnectionClient pLCConnectionClient_;   // PLCConnectionClientのインスタンス
};