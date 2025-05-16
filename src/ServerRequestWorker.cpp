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
        DataLumpBase* sendData;
        {
            std::unique_lock<std::mutex> lock(gSendDataMutex);
            if (gDataLumps.empty()) {
                // cv.wait(lock);
                continue;
            }
            sendData = gDataLumps.front();

            // TODO:単純に消すんじゃなくて別のベクターに格納して消す
            gDataLumps.erase(gDataLumps.begin());
        }

        std::string shapedSendData;
        // 送信データを整形
        shapedSendData = shapeSendData(sendData);

        // shapedSendData = sendData[4] + "," + sendData[0] + "," + sendData[1] + "," + sendData[6];

        // // 確認やつ
        // Logger::getInstance().Info("リストから取り出しました。【SendData】" + shapedSendData);

        // // ログ出力
        // Logger::getInstance().Sensor(shapedSendData);

        // UDPリクエスト 
        if(!serverConnectionClient_.sendMessage(shapedSendData))
        {
            Logger::getInstance().Error("サーバーへのリクエスト送信に失敗しました。");
        }
        else
        {
            Logger::getInstance().Sensor(shapedSendData);
            Logger::getInstance().Info("サーバーへのリクエスト送信に成功しました。");
        }
    }
}

std::string ServerRequestWorker::shapeSendData(const DataLumpBase* sendData) 
{
    std::string shapedSendData;
    std::string timeStamp = Logger::getInstance().GetCurrentTimestampString();
    std::vector<std::map<std::string,std::string>> members;
    std::vector<std::string> sendDataVector;

    // タイムスタンプ整形
    timeStamp.erase(timeStamp.size() - 4);

    shapedSendData += timeStamp + ",";

    // DataLumpBaseのメンバーを送信する文字列に整形する処理
    members = sendData->getMembers();
    bool cateFrag = false;
    for (auto& member : members) {
        std::vector<std::string> tempvector;
        for (auto& pair : member) {
            if (pair.first == "categoryID" || pair.first == "sensorID" || pair.first == "device" || pair.first == "data") {
                tempvector.push_back(pair.second + ",");
                continue;
            }
        }
        // 並び替え
        std::swap(tempvector[1], tempvector[3]);
        sendDataVector.insert(sendDataVector.end(), tempvector.begin(), tempvector.end());
    }

    // sendDataVectorをstringに変換
    for (int i = 0; i < sendDataVector.size(); i++) {
        shapedSendData += sendDataVector[i];
    }

    // 末尾のカンマを削除
    shapedSendData.pop_back();

    // 確認用
    Logger::getInstance().Info("整形したデータ: " + shapedSendData);

    return shapedSendData;
}