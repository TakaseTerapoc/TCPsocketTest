#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "PLCResponseData.hpp"

class PLCRequestData {
public:
    // シリアル番号
    std::string serialNumber;

    // コマンド (例: "write", "read" など)
    std::string command;

    // データアドレス (例: "D200" など)
    std::string dataAddress;

    // デバイス点数
    int deviceCount;

    // 送信間隔（ミリ秒）
    int transmissionIntervalMs;

    // PLCのIPアドレス
    std::string PLCIpAddress;

    // PLCのポート
    int PLCPortNumber;

    // サーバのIPアドレス
    std::string serverIpAddress;

    // サーバのポート
    int serverPortNumber;

    // タイミングを計るときに必要な時間データ
    std::chrono::steady_clock::time_point nextTime;

    // // MCプロトコル
    // char MCprotocol[12];

    // MCプロトコルベクター
    std::vector<char> protocolbuf{20};
    
    // PLCからのレスポンスデータ
    std::vector<PLCResponseData> data;

    //コンストラクタ
    PLCRequestData() = default;

    PLCRequestData(
        std::string serialNumber, 
        std::string command, 
        std::string dataAddress, 
        int deviceCount, 
        int transmissionIntervalMs,
        std::string PLCIpAddress,
        int PLCPortNumber,
        std::string serverIpAddress,
        int serverPortNumber
    );

    // 設定ファイルの情報をMCプロトコルに変換します。
    void convertMCprotocol();
};
