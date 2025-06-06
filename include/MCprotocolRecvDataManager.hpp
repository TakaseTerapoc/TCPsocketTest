#pragma once

#include <vector>
#include <bitset>
#include <string>
#include <array>
#include <iostream>
#include <map>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <iomanip>

#include "Logger.hpp"
#include "PLCTransactionData.hpp"
#include "globals.hpp"
#include "MCprotocolConfigData.hpp"

using namespace std;

namespace FX3UC
{
    class MCprotocolRecvDataManager
    {
        public:
            // シングルトンインスタンスを取得する関数
            static MCprotocolRecvDataManager& getInstance();

            // レスポンスデータを受け取り、送信データに変換する関数
            vector<map<string,string>> convertResponseDataToSendData(char* text, int len, PLCTransactionData& req);

        private:
            // コンストラクタとデストラクタ
            MCprotocolRecvDataManager() = default;
            ~MCprotocolRecvDataManager() = default;

            // コピー禁止
            MCprotocolRecvDataManager(const MCprotocolRecvDataManager&) = delete;
            MCprotocolRecvDataManager& operator=(const MCprotocolRecvDataManager&) = delete;


    };
}