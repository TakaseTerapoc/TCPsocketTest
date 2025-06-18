#include "ServerRequestWorker.hpp"
#include "ServerConnectionClient.hpp"

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

// recv専用スレッドを実行する関数　別スレッド
void ServerRequestWorker::runRecving() {
    while (!gShouldExit) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }
        // vectorをチェック
        if (gResendVectorServerSendDataBuilder.Empty()) {
            //　condition_variableで待機中のrunスレッドに通知
            {
                std::unique_lock<std::mutex> lock(cvMutex_);
                cv_.notify_all();
            }
            // cv.wait(lock);
            continue;
        }
        // recv処理
        char recvBuffer[BuFFER_SIZE];
        unsigned int recvSize = 0;
        if(!serverConnectionClient_.recvMessage(recvBuffer, BuFFER_SIZE, recvSize)) {
            if (gShouldExit) {
                break; // gShouldExitがtrueなら、スレッドを終了
            }
            // エラーコードをチェック
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // タイムアウトエラーの場合は、再試行
                Logger::getInstance().Debug("サーバーからの受信がタイムアウトしました。再試行します。"+ to_string(errno));
                this_thread::sleep_for(chrono::milliseconds(100));
                continue; // タイムアウトエラーの場合は、再試行
            } else {
                // その他のエラーの場合は、ログに記録
                Logger::getInstance().Error("サーバーからの受信中にエラーが発生しました。errno: " + to_string(errno));
                continue; // 受信失敗時は次のループへ
            }
        }
        
        Logger::getInstance().Debug("サーバーからのACK受信に成功しました。受信データ: " + Utilities::convertBinaryHeaderToString(recvBuffer));
        
        // recvで帰ってきたデータのリファレンスナンバーを取得
        // recvBufferの2番目の要素にリファレンスナンバーがある
        char refNum = Utilities::getCharNthElement(recvBuffer, 3, recvSize); // recvSizeのところ、またはBuFFER_SIZE
        
        // refNumが合致するvectorの要素を探す
        int index = Utilities::getVectorCharNthElement(gResendVectorServerSendDataBuilder, refNum);

        // gResendVectorから該当する要素を削除する。
        if (!gResendVectorServerSendDataBuilder.Try_pick(index)) {
            Logger::getInstance().Debug("リファレンスナンバーが見つかりませんでした。リファレンスナンバー: " + to_string(refNum));
        }
        else {
            Logger::getInstance().Debug("リファレンスナンバー: " + to_string(refNum) + " のデータをヴェクターから取り出しました。");
        }

        //　condition_variableで待機中のrunスレッドに通知
        {
            std::unique_lock<std::mutex> lock(cvMutex_);
            cv_.notify_all();
        }
    }
    Logger::getInstance().Debug("終了処理があったため受信スレッドを終了します。");
    //　condition_variableで待機中のrunスレッドに通知
    {
        std::unique_lock<std::mutex> lock(cvMutex_);
        cv_.notify_all();
    }
}

// キューから出し、PLCへのTCPリクエストを依頼する。別スレッド
void ServerRequestWorker::run() {
    while (!gShouldExit) {
        {
            lock_guard<mutex> lg(mutex_);
            if (!running_) break;
        }

        // ServerSendDataBuilderのインスタンスを取得
        ServerSendDataBuilder serverSendDataBuilder;

        char* completedData = nullptr;

        // 再送信データがある場合はそれを送信
        if (!gResendVectorServerSendDataBuilder.Empty()) {
            Logger::getInstance().Debug("再送信データがあるため、vectorから取得します。");
            {
                gResendVectorServerSendDataBuilder.try_pop(serverSendDataBuilder); // 先頭の要素を取り出す
            }
            Logger::getInstance().Debug("再送信データを取得しました。リファレンスナンバー: " + to_string(serverSendDataBuilder.getReferenceNumber()));
        }
        // 送信データがない場合は、gSendDataVectorStrからデータを取得
        else
        {
            Logger::getInstance().Debug("再送信データがないため、gSendDataVectorStrからデータを取得します。");
            string sendData;
            {
                unique_lock<mutex> lock(gSendDataMutex);
                if (gSendDataVectorStr.empty()) {
                    Logger::getInstance().Debug("gSendDataVectorStrが空です。待機します。");
                    gcv.wait(lock);
                    continue;
                }
                sendData = gSendDataVectorStr.front();

                gSendDataVectorStr.erase(gSendDataVectorStr.begin());
            }

            // リファレンスナンバーを設定
            serverSendDataBuilder.setReferenceNumber(referenceNumber++);

            // 送信データにヘッダーを追加して、最終的な送信データを作成
            serverSendDataBuilder.buildPostData(ServerConstData::DATA_TYPE_LITERAL, sendData);

            // 送信データをキューに登録
            {
                gResendVectorServerSendDataBuilder.Push(serverSendDataBuilder);
                Logger::getInstance().Debug("サーバーへの送信データをvectorに登録しました。リファレンスナンバー: " + to_string(serverSendDataBuilder.getReferenceNumber()));
            }
        }
        // デバッグ用出力
#ifdef DEBUG
        string tempStrBinary = Utilities::convertBinaryHeaderToString(serverSendDataBuilder.getPostDataBuf());
        string tempStrText = Utilities::convertTextPayloadToString(serverSendDataBuilder.getPostDataBuf());
        Logger::getInstance().Debug("サーバーへ送られるデータ" + tempStrBinary + tempStrText);
#endif
        // 送信データを文字列に変換
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

        //　condition_variableを使って待機
        {
            Logger::getInstance().Debug("サーバーへの送信後、condition_variableで待機します。");
            std::unique_lock<std::mutex> lock(cvMutex_);
            cv_.wait(lock);
            Logger::getInstance().Debug("condition_variableで待機中のrunスレッドが通知を受け取りました。");
        } 
    }
}