# include "ServerSendDataBuilder.hpp"

// ServerSendDataBuilder& ServerSendDataBuilder::getInstance() {
//     static ServerSendDataBuilder instance;
//     return instance;
// }

# define HEADER_SIZE 7

void ServerSendDataBuilder::buildPostData(ServerConstData::DataType type, const string& sendData) 
{
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
    // dataSizeからヘッダーのサイズ引いたものを16ビットのビッグエンディアン形式に変換
    vector<char> dataSizeBytes = Utilities::decToBytes16ByBigEndian(dataSize - HEADER_SIZE);

    // ヘッダーにデータサイズを追加
    Utilities::appendVectorElements(formatHeader, dataSizeBytes);
}

/**
 * @param データの合計を設定する関数
 * @note データサイズの下位バイトのみをサム値とします。
 */ 
void ServerSendDataBuilder::setDataSum()
{
    formatHeader.push_back(static_cast<char>((dataSize - HEADER_SIZE) & 0xFF)); // 下位バイトのみを追加
}

void ServerSendDataBuilder::setFormatData(const string& sendData)
{
    formatData = Utilities::convertStringToVectorChar(sendData);
}