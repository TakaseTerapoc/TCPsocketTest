#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include "Logger.hpp"
#include "DataLumpBase.hpp"

using namespace std;

class DataLump{
public:
    // コンストラクタ
    DataLump() = default;

    // 送信間隔
    int sendIntervalMs = 0;

    // データが満杯かどうか
    bool lumpFull = false;

    vector<map<string,bool>> sensorReadyStatus;

    vector<map<string,string>> sendData;
};