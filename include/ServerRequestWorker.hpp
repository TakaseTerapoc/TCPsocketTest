#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm> 
#include <iostream>
#include <iomanip>

#include "globals.hpp"
#include "../external/fmt/format.h"
#include "ServerConnectionClient.hpp"
#include "Utilities.hpp"

# define DEBUG

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

        // コピー禁止
        ServerRequestWorker(const ServerRequestWorker&) = delete;
        ServerRequestWorker& operator=(const ServerRequestWorker&) = delete;

        // リストから出して、UDP送信を依頼する。
        void run();

        // 受信スレッドを実行する関数
        void runRecving();  

        // リファレンスナンバーを管理する変数
        int         referenceNumber = 0;

        // 実行スレッド
        thread      thread_;

        // recv専用スレッド
        thread      recvThread_;                                   
        
        // 実行中フラグ
        atomic<bool>  running_{false};

        // 準備完了フラグ
        atomic<bool>  recvReady_{false};                           
        
        // running_ の排他制御
        mutex       mutex_;

        // condition_variableのmutex
        mutex       cvMutex_;

        // condition_variable;
        condition_variable cv_;                                    

        // ServerConnectionClientのインスタンス
        ServerConnectionClient serverConnectionClient_;

        // serverへの送信ヘッダデータをstringにする関数             
        string buildBinaryHeaderString(const char* data, size_t headerSize = 7);

        // serverへの送信テキストデータをstringにする関数
        string buildTextPayloadString(const char* data, size_t offset = 7);

};