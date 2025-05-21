#pragma once

#include "Logger.hpp"
#include "AppConfig.hpp"
#include "CsvReader.hpp"
#include "CSVIO.hpp"
#include "MCprotocolManager.hpp"
#include "PLCConnectionClient.hpp"
#include "ServerConnectionClient.hpp"
#include "PLCRequestScheduler.hpp"
#include "PLCRequestWorker.hpp"
#include "ServerRequestWorker.hpp"

#include <csignal>
#include <atomic>
#include <thread>

using namespace std;

class AppController {
public:
    // コンストラクタ・デストラクタ
    AppController();
    ~AppController();

    // アプリケーション起動
    void run();

    // シグナルハンドラから呼び出される停止処理
    void stop();  

private:
    // ロガーの初期化
    void initLogger();

    // 設定ファイルの読み込み
    void loadConfig();

    // PLCリクエストデータの準備
    void prepareRequestData();

    // PLCおよびサーバーへの接続
    void setupConnections();

    // スケジューラとワーカーの起動
    void startWorkers();

    // シャットダウン待機
    void waitForShutdown();

    PLCConnectionClient* plcConnectionClient_;
    ServerConnectionClient* serverConnectionClient_;
};
