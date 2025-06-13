#include "ServerRequestWorker.hpp"

# define DEBUG

// シングルトンインスタンス取得
ServerRequestWorker& ServerRequestWorker::getInstance(ServerConnectionClient& serverClient) {
    static ServerRequestWorker instance;
    instance.serverConnectionClient_ = serverClient;
    return instance;
}

// ワーカー開始
void ServerRequestWorker::start() {
    lock_guard<mutex> lock(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = thread(&ServerRequestWorker::run, this);
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
        lock_guard<mutex> lock(mutex_);
        running_ = false;
    }
    join();
}

// キューから出し、PLCへのTCPリクエストを依頼する。
void ServerRequestWorker::run() {
    while (!gShouldExit) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }
        vector<map<string,string>> sendData;
        {
            unique_lock<mutex> lock(gSendDataMutex);
            if (gSendDataMap.empty()) {
                // cv.wait(lock);
                continue;
            }
            sendData = gSendDataMap.front();

            // TODO:単純に消すんじゃなくて別のベクターに格納して消す
            gSendDataMap.erase(gSendDataMap.begin());
        }
        
        // 送信データを整形
        const string shapedSendData = ServerSendDataBuilder::getInstance().shapeSendData(sendData);

        // 送信データにヘッダーを追加して、最終的な送信データを作成
        char* completedData = ServerSendDataBuilder::getInstance().buildPostData(ServerConstData::DATA_TYPE_LITERAL, shapedSendData);

#ifdef DEBUG
        string tempStrBinary = buildBinaryHeaderString(completedData);
        string tempStrText = buildTextPayloadString(completedData);
        Logger::getInstance().Debug("サーバーへ送られるデータ" + tempStrBinary + tempStrText);
#endif
        // UDPリクエスト 
        if(!serverConnectionClient_.sendMessage(completedData, ServerSendDataBuilder::getInstance().getDataSize()))
        {
            string completedDataStr(completedData);
            completedDataStr = "【サーバー送信失敗】" + completedDataStr;
            Logger::getInstance().Error("サーバーへの送信に失敗しました。");
        }
        else
        {
            Logger::getInstance().Debug("サーバーへの送信に成功しました。");
        }
        Logger::getInstance().Sensor(shapedSendData);
        
        // 送信後にデータを初期化
        ServerSendDataBuilder::getInstance().initializeData();
    }
}

std::string ServerRequestWorker::buildBinaryHeaderString(const char* data, size_t headerSize) {
    std::ostringstream oss;
    oss << "Header (hex): ";
    for (size_t i = 0; i < 7; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(static_cast<unsigned char>(data[i])) << " ";
    }
    return oss.str();
}

std::string ServerRequestWorker::buildTextPayloadString(const char* data, size_t offset) {
    std::ostringstream oss;
    oss << "Payload (text): " << (data + offset);
    return oss.str();
}