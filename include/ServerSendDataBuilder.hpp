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
    static ServerSendDataBuilder& getInstance();

    // POSTデータを構築する関数
    char* buildPostData(ServerConstData::DataType type, const string& sendData = "");

    // サーバーに送るデータをstringに整形する関数
    string shapeSendData(const vector<map<string,string>>& sendData); 

    // データサイズを取得する関数
    unsigned int getDataSize() const {
        return dataSize;
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
    // コンストラクタとデストラクタ
    ServerSendDataBuilder() = default;
    ~ServerSendDataBuilder() = default;

    // コピー禁止
    ServerSendDataBuilder(const ServerSendDataBuilder&) = delete;
    ServerSendDataBuilder& operator=(const ServerSendDataBuilder&) = delete;

    // POSTデータ
    char* postDataBuf;

    string postDataString;

    // ヘッダーデータ
    vector<char> formatHeader;

    // フォーマットデータ
    vector<char> formatData;

    // 通番(0x00~0xFFFF)を管理する変数
    unsigned int referenceNumber = 0;

    // データサイズ
    unsigned int dataSize = 0;

    //　vector<map<string,string>>を整形して文字列に変換する関数 
    vector<string> shapeSendDataVector(const vector<map<string,string>>& sendData);

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