#include "AppController.hpp"

// シグナルハンドラ（Ctrl+C / kill の終了に反応）
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        Logger::getInstance().Debug("終了シグナルを受信しました。終了処理を行います。");
        gShouldExit = true;

        // if (gAppInstance != nullptr) {
        //     gAppInstance->stop();
        // }
    }
}

AppController::AppController()
    : plcConnectionClient_(nullptr), serverConnectionClient_(nullptr)
{
    gAppInstance = this;
}

AppController::~AppController() {
    // stop();
    delete plcConnectionClient_;
    delete serverConnectionClient_;
    gAppInstance = nullptr;
}

void AppController::run() {
    // 【Ctrl+C】 されたら signalHandler() を呼ぶ
    signal(SIGINT, signalHandler);

    // 【kill】 されたら signalHandler() を呼ぶ
    // ただし、kill -9 などの強制終了は捕捉できない
    // 参考: https://www.oreilly.co.jp/books/9784873117980/
    signal(SIGTERM, signalHandler);

    signal(SIGPIPE,SIG_IGN);

    // アプリケーションの起動時に必要な初期化処理を行う
    initLogger();
    loadConfig();
    prepareRequestData();

    // PLC接続とサーバー接続の設定を行う・接続ができない場合は、if内の処理は行わない。
    // TODO:サーバーとのハンドシェイク処理を追加する。
    if(setupConnections())
    {
        startWorkers();
        waitForShutdown();
    }
}

void AppController::initLogger() {
    Logger::getInstance().Init();
    Logger::getInstance().Debug("ロガー初期化完了");
    Logger::getInstance().Debug("アプリが起動しました。");
}

void AppController::loadConfig() {
    
    Logger::getInstance().Debug("設定ファイルを読み込みます。");
    
    if (!AppConfig::getInstance().LoadFiles(FileResources::FILEPATHCONFIGFILE)) {
        Logger::getInstance().Error("設定ファイルの読込に失敗しました。");
        exit(1);
    }
    Logger::getInstance().Debug("設定ファイルの読込完了。");
}

void AppController::prepareRequestData() {
    PLCTransactionDataBuilder plcTransactionDataBuilder;

    Logger::getInstance().Debug("PLCリクエストデータの準備を開始します。");
    
    // CSVファイルを読み込み
    auto mapdata = CsvReader::readCSVFileToMapVector(AppConfig::getInstance().requestFilePath);
    
    // CSVデータをPLCTransactionDataBuilderでPLCTransactionDataに変換
    gRData = plcTransactionDataBuilder.makeRequestDataFromMapdata(mapdata);

    // 変換したデータをMCプロトコルのデータに変換
    MCprotocolSendDataManager::getInstance().covertToMCprotocolData(gRData);
    
    Logger::getInstance().Debug("PLCリクエストデータ準備完了しました。");
}

bool AppController::setupConnections() {
    Logger::getInstance().Debug("PLCおよびサーバーに接続を試みます。");

    //　PLC接続
    plcConnectionClient_ = new PLCConnectionClient(
        AppConfig::getInstance().GetPLCConfig("ipaddress").c_str(),
        stoi(AppConfig::getInstance().GetPLCConfig("port"))
    );

    // PLC接続エラーが起きたときに何度もリトライするようにする。
    int result = -1;
    while ((result = plcConnectionClient_->Connect()) < 0 && !gShouldExit) {
        Logger::getInstance().Error2("PLC接続再試行中...",__FILE__, __LINE__, __FUNCTION__);
        this_thread::sleep_for(chrono::seconds(1));
    }
    if (gShouldExit) {
        Logger::getInstance().Debug("終了要求を検出したため、接続処理を中止します。");
        return false;
    }
    Logger::getInstance().Debug("PLC接続に成功しました。");

    // サーバー接続
    // TOdo: サーバー接続確認の実装を追加する
    // 現在は接続確認なし
    serverConnectionClient_ = new ServerConnectionClient(
        AppConfig::getInstance().GetServerConfig("ipaddress").c_str(),
        stoi(AppConfig::getInstance().GetServerConfig("port"))
    );
    return true;
}

void AppController::startWorkers() {
    Logger::getInstance().Debug("スケジューラとワーカーを起動します。");

    PLCRequestScheduler::getInstance().start();
    PLCRequestWorker::getInstance(*plcConnectionClient_).start();
    ServerRequestWorker::getInstance(*serverConnectionClient_).start();
}

void AppController::waitForShutdown() {
    Logger::getInstance().Debug("Ctrl+Cまたはkillで終了できます。終了を待機中...");

    while (!gShouldExit) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    stop();
}

void AppController::stop() {
    Logger::getInstance().Debug("スレッドにstop信号を送信します。");
    if (plcConnectionClient_) {
        PLCRequestWorker::getInstance(*plcConnectionClient_).stop();
    }

    PLCRequestScheduler::getInstance().stop();

    if (serverConnectionClient_) {
        ServerRequestWorker::getInstance(*serverConnectionClient_).stop();
    }
}