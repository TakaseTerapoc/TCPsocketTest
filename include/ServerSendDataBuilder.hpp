#pragma once

#include "Logger.hpp"
#include "Utilities.hpp"
#include "ServerConstData.hpp"

using namespace std;

/**
 * @brief サーバーに送るデータを加工するクラスです。
*/
class ServerSendDataBuilder {
public:
    // シングルトンインスタンス取得
    // static ServerSendDataBuilder& getInstance();

    // コンストラクタとデストラクタ
    ServerSendDataBuilder() = default;
    ServerSendDataBuilder(int Number) 
    {
        referenceNumber = Number;
    }

    ~ServerSendDataBuilder() = default;

    // POSTデータを構築する関数
    void buildPostData(ServerConstData::DataType type, const string& sendData = "");

    // サーバーに送るデータをstringに整形する関数
    void shapeSendData(const vector<map<string,string>>& sendData); 

    // データサイズを取得する関数
    unsigned int getDataSize() const {
        return dataSize;
    }

    // postDataBufを取得する関数 
    char* getPostDataBuf() const {
        return postDataBuf;
    }

    // referenceNumberを取得する関数
    unsigned int getReferenceNumber() const {
        return referenceNumber;
    }

    // referenceNumberをセットする関数
    void setReferenceNumber(unsigned int number) {
        // 0x00~0xFFFFの範囲に収める
        if (number > 0xFFFF) {
            number = 0;
        }
        referenceNumber = number;
    }

    // shapedSendDataStringを取得する関数
    string getShapedSendDataString() const {
        return shapedSendDataString;
    }

    // 各データを初期化する関数
    void initializeData() {
        postDataBuf = nullptr;
        postDataString.clear();
        formatHeader.clear();
        formatData.clear();
        dataSize = 0;
    }

private:

    // // コピー禁止
    // ServerSendDataBuilder(const ServerSendDataBuilder&) = delete;
    // ServerSendDataBuilder& operator=(const ServerSendDataBuilder&) = delete;

    // POSTデータ
    char* postDataBuf;

    // 成形したsendDataを保持する文字列
    string shapedSendDataString;

    string postDataString;

    // ヘッダーデータ
    vector<char> formatHeader;

    // フォーマットデータ
    vector<char> formatData;

    // 通番(0x00~0xFFFF)を管理する変数
    unsigned int referenceNumber = 0;

    // データサイズ
    unsigned int dataSize = 0;

    // ヘッダーデータを構築する関数
    void buildHeaderData(ServerConstData::DataType type, const string& sendData);

    // パケットタイプを設定する関数
    void setPacketType();

    // データタイプを設定する関数
    void setDataType(ServerConstData::DataType type);

    // 通番を設定する関数
    void setReferenceNumber();

    // データサイズを設定する関数
    void setDataSize(const string& sendData);

    // データの合計を設定する関数
    void setDataSum();

    // データを設定する関数
    void setFormatData(const string& sendData);

};