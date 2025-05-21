#include "AppController.hpp"

// シグナルハンドラ（Ctrl+C / kill の終了に反応）
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        Logger::getInstance().Info("終了シグナルを受信しました。終了処理を行います。");
        gShouldExit = true;

        if (gAppInstance != nullptr) {
            gAppInstance->stop();
        }
    }
}

AppController::AppController()
    : plcConnectionClient_(nullptr), serverConnectionClient_(nullptr)
{
    gAppInstance = this;
}

AppController::~AppController() {
    stop();
    delete plcConnectionClient_;
    delete serverConnectionClient_;
    gAppInstance = nullptr;
}

void AppController::run() {
    // 【Ctrl+C】 されたら signalHandler() を呼ぶ
    std::signal(SIGINT, signalHandler);

    // 【kill】 されたら signalHandler() を呼ぶ
    // ただし、kill -9 などの強制終了は捕捉できない
    // 参考: https://www.oreilly.co.jp/books/9784873117980/
    std::signal(SIGTERM, signalHandler);

    initLogger();
    loadConfig();
    prepareRequestData();
    setupConnections();
    startWorkers();
    waitForShutdown();
}

void AppController::initLogger() {
    Logger::getInstance().Init();
    Logger::getInstance().Info("ロガー初期化完了");
}

void AppController::loadConfig() {
    Logger::getInstance().Info("設定ファイルを読み込みます。");
    if (!AppConfig::getInstance().LoadFile("../ini/config.ini")) {
        Logger::getInstance().Error("設定ファイルの読込に失敗しました。");
        exit(1);
    }
    Logger::getInstance().Info("設定ファイルの読込完了。");
}

void AppController::prepareRequestData() {
    Logger::getInstance().Info("PLCリクエストデータの準備を開始します。");

    auto mapdata = CSVIO::readCSVFileToMapVector("../request/testdataDemo.csv");
    gRData = CSVIO::makeRequestDataFromMapdata(mapdata);

    MCprotocolManager::covertToMCprotocolData2(gRData);
    Logger::getInstance().Info("PLCリクエストデータ準備完了。");
}

void AppController::setupConnections() {
    Logger::getInstance().Info("PLCおよびサーバーに接続を試みます。");

    //　PLC接続
    plcConnectionClient_ = new PLCConnectionClient(
        AppConfig::getInstance().GetPLCConfig("ipaddress").c_str(),
        std::stoi(AppConfig::getInstance().GetPLCConfig("port"))
    );

    // TODO:PLC接続エラーが起きたときに何度もリトライするようにする。
    // 現在は未完成
    int result = -1;
    size_t retry = 0;
    while ((result = plcConnectionClient_->Connect()) < 0) {
        if (retry++ > 0) {
            Logger::getInstance().Error("PLC接続再試行中...");
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    Logger::getInstance().Info("PLC接続に成功しました。");
    
    // サーバー接続
    // TOdo: サーバー接続確認の実装を追加する
    // 現在は接続確認なし
    serverConnectionClient_ = new ServerConnectionClient(
        AppConfig::getInstance().GetServerConfig("ipaddress").c_str(),
        std::stoi(AppConfig::getInstance().GetServerConfig("port"))
    );
}

void AppController::startWorkers() {
    Logger::getInstance().Info("スケジューラとワーカーを起動します。");

    PLCRequestScheduler::getInstance().start();
    PLCRequestWorker::getInstance(*plcConnectionClient_).start();
    ServerRequestWorker::getInstance(*serverConnectionClient_).start();
}

void AppController::waitForShutdown() {
    Logger::getInstance().Info("Ctrl+Cまたはkillで終了できます。終了を待機中...");

    while (!gShouldExit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Logger::getInstance().Info("終了処理を開始します。");
    PLCRequestWorker::getInstance(*plcConnectionClient_).join();
    PLCRequestScheduler::getInstance().join();
    ServerRequestWorker::getInstance(*serverConnectionClient_).join();
}

void AppController::stop() {
    Logger::getInstance().Info("スレッドにstop信号を送信します。");
    if (plcConnectionClient_) {
        PLCRequestWorker::getInstance(*plcConnectionClient_).stop();
    }

    PLCRequestScheduler::getInstance().stop();

    if (serverConnectionClient_) {
        ServerRequestWorker::getInstance(*serverConnectionClient_).stop();
    }
}