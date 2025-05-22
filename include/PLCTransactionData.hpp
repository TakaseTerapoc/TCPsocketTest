#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "Logger.hpp"

using namespace std;

class PLCTransactionData {
public:
    // シリアル番号
    string serialNumber;

    // コマンド (例: "write", "read" など)
    string command = "read";

    // データアドレス (例: "D200" など)
    string dataAddress;

    // デバイスコード (例: "D", "M", "C" など)
    string deviceCode;

    // デバイス点数
    int deviceCount;

    // データ間隔 (一つのリクエストに対して複数のデータがある場合、どれくらいの間隔でデータが存在するか。)
    vector<int> dataInterval;

    // 送信間隔（ミリ秒）
    int sendIntervalMs;

    // タイミングを計るときに必要な時間データ
    chrono::steady_clock::time_point nextTime;

    /* csvdataを格納するための変数
     * sensorrows[n] = { SerialNumber, DeviceAddress, DeviceAddressAscii, Comment }
     */ 
    vector<vector<string>> sensorrows;

    /* mapdataを格納するためのリスト
     * sensorrows[n] = { SerialNumber, DeviceAddress, DeviceAddressAscii, Comment }
     */ 
    vector<map<string, string>> mapdata;

    // MCプロトコル
    vector<char> protocolbuf{20};

    // PLCレスポンスデータの受信時間です。
    string receiptTime;        
    
    // PLCレスポンスデータです。
    string responseData;

    // サーバに送信する際のデータです。
    vector<vector<string>> sendData;

    // コンストラクタ
    PLCTransactionData() = default;
};
