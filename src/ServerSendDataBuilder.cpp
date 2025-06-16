# include "ServerSendDataBuilder.hpp"

// ServerSendDataBuilder& ServerSendDataBuilder::getInstance() {
//     static ServerSendDataBuilder instance;
//     return instance;
// }

void ServerSendDataBuilder::shapeSendData(const vector<map<string,string>>& sendData) 
{
    string timeStamp = Logger::getInstance().GetCurrentTimestampString();
    vector<string> sendDataVector;

    // タイムスタンプ整形
    timeStamp.erase(timeStamp.size() - 4);

    shapedSendDataString += timeStamp + ",";

    // sendDataのメンバーを送信する文字列に並び替え、整形する処理
    sendDataVector = shapeSendDataVector(sendData);
    
    // 送信データを整形
    shapedSendDataString += Utilities::convertVectorStringToString(sendDataVector);

    // 末尾のカンマを削除
    shapedSendDataString.pop_back();

    // 確認用
    Logger::getInstance().Debug("整形したデータ: " + shapedSendDataString);
}

vector<string> ServerSendDataBuilder::shapeSendDataVector(const vector<map<string,string>>& sendData) 
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

void ServerSendDataBuilder::buildPostData(ServerConstData::DataType type, const string& sendData) 
{
    // referenceNumberが0xFFFFを超えた場合は0にリセット
    if (referenceNumber > 0xFFFF) {
        referenceNumber = 0;
    }
    // フォーマットヘッダーを作成
    buildHeaderData(type, sendData);

    // フォーマットデータを設定
    setFormatData(sendData);

    // ヘッダーデータとフォーマットデータを結合し、postDataBufに変換
    vector<char>tempPostData = Utilities::appendVectorElements(formatHeader, formatData);
    postDataString.assign(tempPostData.begin(), tempPostData.end());
    postDataBuf = const_cast<char*>(postDataString.c_str());
}

void ServerSendDataBuilder::buildHeaderData(ServerConstData::DataType type, const string& sendData) 
{
    formatHeader.clear();
    setPacketType();
    setDataType(type);
    setReferenceNumber();
    setDataSize(sendData);
    setDataSum();
}


void ServerSendDataBuilder::setPacketType()
{
    formatHeader.push_back(0x00);
}

// データタイプを設定する関数
void ServerSendDataBuilder::setDataType(ServerConstData::DataType type)
{
    formatHeader.push_back(static_cast<char>(type));
}

// 通番を設定する関数
void ServerSendDataBuilder::setReferenceNumber()
{
    Utilities::appendVectorElements(formatHeader, Utilities::decToBytes16ByBigEndian(referenceNumber));
    referenceNumber++;
}

/**
 * @brief データサイズを設定する関数
 * @param sendData 送る文字列データ　TODO:後でフォーマットが加わる予定
*/
void ServerSendDataBuilder::setDataSize(const string& sendData)
{
    int formatDataSize = sendData.size();
    dataSize = ServerConstData::IpHeaderSize + ServerConstData::UDPHeaderSize + ServerConstData::formatHeaderSize + formatDataSize;
    if (dataSize > 0xFFDC) {
        Logger::getInstance().Error("データサイズが大きすぎます: " + to_string(dataSize));
        exit(1);
    }
    // dataSizeを16ビットのビッグエンディアン形式に変換
    vector<char> dataSizeBytes = Utilities::decToBytes16ByBigEndian(dataSize);

    // ヘッダーにデータサイズを追加
    Utilities::appendVectorElements(formatHeader, dataSizeBytes);
}

/**
 * @param データの合計を設定する関数
 * @note データサイズの下位バイトのみをサム値とします。
 */ 
void ServerSendDataBuilder::setDataSum()
{
    formatHeader.push_back(static_cast<char>(dataSize & 0xFF)); // 下位バイトのみを追加
}

void ServerSendDataBuilder::setFormatData(const string& sendData)
{
    formatData = Utilities::convertStringToVectorChar(sendData);
}