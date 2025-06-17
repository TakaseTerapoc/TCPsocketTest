#include "ServerRequestWorker.hpp"

# define DEBUG
# define BuFFER_SIZE 16

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
    recvThread_ = thread(&ServerRequestWorker::runRecving, this); // recv専用スレッドも開始
}

// ワーカjoin
void ServerRequestWorker::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
    if (recvThread_.joinable()) {
        recvThread_.join();
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

void ServerRequestWorker::runRecving() {
    while (!gShouldExit) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }
        // vectorをチェック
        if (gResendVectorServerSendDataBuilder.Empty()) {
            // cv.wait(lock);
            continue;
        }

        // recv処理
        char recvBuffer[BuFFER_SIZE];
        unsigned int recvSize = 0;
        if (!serverConnectionClient_.recvMessage(recvBuffer, BuFFER_SIZE, recvSize)) {
            Logger::getInstance().Error("サーバーからの受信に失敗しました。");
            continue; // 受信失敗時は次のループへ
        }
        
        // recvで帰ってきたデータのリファレンスナンバーを取得
        // recvBufferの2番目の要素にリファレンスナンバーがある
        char refNum = Utilities::getCharNthElement(recvBuffer, 2, recvSize); // recvSizeのところ、またはBuFFER_SIZE
        
        // refNumが合致するvectorの要素を探す
        int index = Utilities::getVectorCharNthElement(gResendVectorServerSendDataBuilder, refNum);

        // gResendVectorから該当する要素を削除する。
        if (!gResendVectorServerSendDataBuilder.Try_pick(index)) {
            Logger::getInstance().Debug("リファレンスナンバーが見つかりませんでした。リファレンスナンバー: " + to_string(refNum));
        }
        else {
            Logger::getInstance().Debug("リファレンスナンバー: " + to_string(refNum) + " のデータをキューから取り出しました。");
        }

        //　condition_variableで待機中のrunスレッドに通知
        {
            std::unique_lock<std::mutex> lock(cvMutex_);
            cv_.notify_all();
        }
    }
}

// キューから出し、PLCへのTCPリクエストを依頼する。
void ServerRequestWorker::run() {
    while (!gShouldExit) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }

        // リファレンスナンバーが0xFFFFを超えた場合は0にリセット
        if (referenceNumber > 0xFFFF) {
            referenceNumber = 0;
        }

        // ServerSendDataBuilderのインスタンスを取得
        ServerSendDataBuilder serverSendDataBuilder(referenceNumber);

        char* completedData = nullptr;

        // 再送信データがある場合はそれを送信
        if (!gResendVectorServerSendDataBuilder.Empty()) {
            {
                gResendVectorServerSendDataBuilder.try_pop(serverSendDataBuilder); // 先頭の要素を取り出す
            }
            Logger::getInstance().Debug("再送信データを取得しました。リファレンスナンバー: " + serverSendDataBuilder.getReferenceNumber());
        }
        // 送信データがない場合は、gSendDataMapからデータを取得
        else
        {
            vector<map<string,string>> sendData;
            {
                unique_lock<mutex> lock(gSendDataMutex);
                if (gSendDataMap.empty()) {
                    // cv.wait(lock);
                    continue;
                }
                sendData = gSendDataMap.front();

                gSendDataMap.erase(gSendDataMap.begin());
            }
            
            // 送信データを整形→メンバ関数shapedSendDataStringに格納
            serverSendDataBuilder.shapeSendData(sendData);

            // 送信データにヘッダーを追加して、最終的な送信データを作成
            serverSendDataBuilder.buildPostData(ServerConstData::DATA_TYPE_LITERAL, serverSendDataBuilder.getShapedSendDataString());
        }
#ifdef DEBUG
        string tempStrBinary = buildBinaryHeaderString(serverSendDataBuilder.getPostDataBuf());
        string tempStrText = buildTextPayloadString(serverSendDataBuilder.getPostDataBuf());
        Logger::getInstance().Debug("サーバーへ送られるデータ" + tempStrBinary + tempStrText);
#endif
        string completedDataStr(serverSendDataBuilder.getPostDataBuf());

        // UDPリクエスト 
        if(!serverConnectionClient_.sendMessage(serverSendDataBuilder.getPostDataBuf(), serverSendDataBuilder.getDataSize()))
        {
            
            completedDataStr = "【サーバー送信失敗】" + completedDataStr;
            Logger::getInstance().Error("サーバーへの送信に失敗しました。");
        }
        else
        {
            Logger::getInstance().Debug("サーバーへの送信に成功しました。");
        }
        Logger::getInstance().Sensor(completedDataStr);

        // 送信データをキューに登録
        {
            gResendVectorServerSendDataBuilder.Push(serverSendDataBuilder);
            Logger::getInstance().Debug("サーバーへの送信データをvectorに登録しました。リファレンスナンバー: " + to_string(serverSendDataBuilder.getReferenceNumber()));
        }

        referenceNumber++;

        //　condition_variableを使って待機
        {
            std::unique_lock<std::mutex> lock(cvMutex_);
            cv_.wait(lock);
        } 
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