#include "ServerRequestWorker.hpp"

// シングルトンインスタンス取得
ServerRequestWorker& ServerRequestWorker::getInstance(ServerConnectionClient& serverClient) {
    static ServerRequestWorker instance;
    instance.serverConnectionClient_ = serverClient;
    return instance;
}

// ワーカー開始
void ServerRequestWorker::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&ServerRequestWorker::run, this);
}

// ワーカjoin
void ServerRequestWorker::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

// ワーカー停止
void ServerRequestWorker::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }
    join();
}

// キューから出し、PLCへのTCPリクエストを依頼する。
void ServerRequestWorker::run() {
    while (true) {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            if (!running_) break;
        }
        std::vector<std::string> sendData;
        {
            std::unique_lock<std::mutex> lock(gSendDataMutex);
            if (gSendData.empty()) {
                // cv.wait(lock);
                continue;
            }
            sendData = gSendData.front();

            // TODO:単純に消すんじゃなくて別のベクターに格納して消す
            gSendData.erase(gSendData.begin());
        }

        std::string shapedSendData;
        shapedSendData = sendData[4] + "," + sendData[0] + "," + sendData[1] + "," + sendData[6];

        // 確認やつ
        Logger::getInstance().Info("リストから取り出しました。【SendData】" + shapedSendData);

        // ログ出力
        Logger::getInstance().Sensor(shapedSendData);

        // UDPリクエスト 
        if(!serverConnectionClient_.sendMessage(shapedSendData))
        {
            Logger::getInstance().Error("サーバーへのリクエスト送信に失敗しました。");
        }
        else
        {
            Logger::getInstance().Info("サーバーへのリクエスト送信に成功しました。");
        }
    }
}