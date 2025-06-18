#include "DataLump.hpp"
#include "TimeStampManager.hpp"
#include "Utilities.hpp"

void DataLump::isLumpFull() {
    for (const auto& sensorStatus : sensorReadyStatus) {
        for (const auto& [sensorID, status] : sensorStatus) {
            isSendReady = true;
            if (!status) {
                isSendReady = false;
            }
        }
    }
}

void DataLump::allClear() {
        isSendReady = false;
        sendData.clear();
        
        // センサーの準備状況をクリア
        for (auto& sensorStatus : sensorReadyStatus) {
            for (auto& [sensorID, status] : sensorStatus) {
                status = false;
            }
        }
}

string DataLump::shapeSendData(const vector<map<string,string>>& sendData) 
{
    vector<string> sendDataVector;
    string shapedSendDataString;

    // タイムスタンプ取得
    string timeStamp = TimeStampManager::GetCurrentTimestampStringSec();

    shapedSendDataString += timeStamp + ",";

    // sendDataのメンバーを送信する文字列に並び替え、整形する処理
    sendDataVector = shapeSendDataVector(sendData);
    
    // 送信データを整形
    shapedSendDataString += Utilities::convertVectorStringToString(sendDataVector);

    // 末尾のカンマを削除
    shapedSendDataString.pop_back();

    // 確認用
    Logger::getInstance().Debug("整形したデータ: " + shapedSendDataString);

    return shapedSendDataString;
}

vector<string> DataLump::shapeSendDataVector(const vector<map<string,string>>& sendData) 
{
    vector<string> sendDataVector;

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

    return sendDataVector;
}