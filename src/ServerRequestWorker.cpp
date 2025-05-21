#include "ServerRequestWorker.hpp"

using namespace std;

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
    while (true) {
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

        string shapedSendData;
        // 送信データを整形
        shapedSendData = shapeSendData(sendData);

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
string ServerRequestWorker::shapeSendData(const vector<map<string,string>>& sendData) 
{
    string shapedSendData;
    string timeStamp = Logger::getInstance().GetCurrentTimestampString();
    vector<string> sendDataVector;

    // タイムスタンプ整形
    timeStamp.erase(timeStamp.size() - 4);

    shapedSendData += timeStamp + ",";

    // sendDataのメンバーを送信する文字列に整形する処理
    for (auto& row : sendData) {
        vector<string> tempvector;
        for (auto& pair : row) {
            if (pair.first == "categoryID" || pair.first == "sensorID" || pair.first == "device" || pair.first == "data") {
                tempvector.push_back(pair.second + ",");
            }
        }
        swap(tempvector[1], tempvector[3]);
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


// string ServerRequestWorker::shapeSendData2(const DataLumpBase* sendData) 
// {
//     string shapedSendData;
//     string timeStamp = Logger::getInstance().GetCurrentTimestampString();
//     vector<map<string,string>> members;
//     vector<string> sendDataVector;

//     // タイムスタンプ整形
//     timeStamp.erase(timeStamp.size() - 4);

//     shapedSendData += timeStamp + ",";

//     // DataLumpBaseのメンバーを送信する文字列に整形する処理
//     members = sendData->getMembers();
//     bool cateFrag = false;
//     for (auto& member : members) {
//         vector<string> tempvector;
//         for (auto& pair : member) {
//             if (pair.first == "categoryID" || pair.first == "sensorID" || pair.first == "device" || pair.first == "data") {
//                 tempvector.push_back(pair.second + ",");
//                 continue;
//             }
//         }
//         // 並び替え
//         swap(tempvector[1], tempvector[3]);
//         sendDataVector.insert(sendDataVector.end(), tempvector.begin(), tempvector.end());
//     }

//     // sendDataVectorをstringに変換
//     for (int i = 0; i < sendDataVector.size(); i++) {
//         shapedSendData += sendDataVector[i];
//     }

//     // 末尾のカンマを削除
//     shapedSendData.pop_back();

//     // 確認用
//     Logger::getInstance().Info("整形したデータ: " + shapedSendData);

//     return shapedSendData;
// }