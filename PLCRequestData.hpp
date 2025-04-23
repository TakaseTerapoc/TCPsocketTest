#pragma once

#include <string>
#include <vector>
#include "PLCData.hpp"

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
    int sendIntervalMs;

    // PLCのIPアドレス
    std::string PLCIpAddress;

    // PLCのポート
    int PLCPortNumber;

    // サーバのIPアドレス
    std::string serverIpAddress;

    // サーバのポート
    int serverPortNumber;
    
    // PLCからのレスポンスデータ
    std::vector<PLCData> data;

    //コンストラクタ
    PLCRequestData(
        std::string serialNumber, 
        std::string command, 
        std::string dataAddress, 
        int deviceCount, 
        int sendIntervalMs,
        std::string PLCIpAddress,
        int PLCPortNumber,
        std::string serverIpAddress,
        int serverPortNumber
    );
};
