#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm> 

#include "globals.hpp"
#include "../external/fmt/format.h"
#include "ServerConnectionClient.hpp"

using namespace std;

/**
 * @brief SendDataが格納されているリストを監視し、サーバーへデータを送るクラスです。
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

        // リストから出して、UDP送信を依頼する。
        void run();  

        thread      thread_;                                   // 実行スレッド
        bool             running_{false};                           // 実行中フラグ
        mutex       mutex_;                                    // running_ の排他制御
        ServerConnectionClient serverConnectionClient_;             // ServerConnectionClientのインスタンス
        string shapeSendData(const vector<map<string,string>>& sendData);  // DataLumpBaseのメンバーを送信する文字列に整形する関数
};