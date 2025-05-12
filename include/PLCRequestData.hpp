#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "PLCResponseData.hpp"
#include "Logger.hpp"

class PLCRequestData {
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

    // csvdataを格納するための変数
    std::vector<std::vector<std::string>> csvrows;

    // MCプロトコル
    std::vector<char> protocolbuf{20};
    
    // PLCからのレスポンスデータ
    std::vector<PLCResponseData> data;

    // コンストラクタ
    PLCRequestData() = default;

    // 設定ファイルの情報をMCプロトコルに変換します。
    void convertMCprotocol();

private:
    char firstChar(const std::string& s);

};
