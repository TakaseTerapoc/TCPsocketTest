#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "Logger.hpp"

class PLCRequestResponseData {
public:
    // シリアル番号
    std::string serialNumber;

    // コマンド (例: "write", "read" など)
    std::string command = "read";

    // データアドレス (例: "D200" など)
    std::string dataAddress;

    // デバイスコード (例: "D", "M", "C" など)
    std::string deviceCode;

    // デバイス点数
    int deviceCount;

    // データ間隔 (一つのリクエストに対して複数のデータがある場合、どれくらいの間隔でデータが存在するか。)
    std::vector<int> dataInterval;

    // 送信間隔（ミリ秒）
    int transmissionIntervalMs;

    // タイミングを計るときに必要な時間データ
    std::chrono::steady_clock::time_point nextTime;

    /* csvdataを格納するための変数
     * sensorrows[n] = { SerialNumber, DeviceAddress, DeviceAddressAscii, Comment }
     */ 
    std::vector<std::vector<std::string>> sensorrows;

    // MCプロトコル
    std::vector<char> protocolbuf{20};

    // PLCレスポンスデータの受信時間です。
    std::string receiptTime;        
    
    // PLCレスポンスデータです。
    std::string responseData;

    // サーバに送信する際のデータです。
    std::vector<std::vector<std::string>> sendData;

    // コンストラクタ
    PLCRequestResponseData() = default;
};
