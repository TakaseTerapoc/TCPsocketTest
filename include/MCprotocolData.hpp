#pragma once

#include <vector>
#include "PLCRequestData.hpp"
#include <string>
#include <array>
#include <iostream>
#include <map>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <iomanip>
#include "Logger.hpp"

class MCprotocolData
{
    public:
        //コンストラクタ
        MCprotocolData() = default;

        // PLCRequestDataの各種メンバーからMCプロトコルデータに変換する関数
        void covertToMCprotocolData(std::vector<PLCRequestData>& gRData);

    private:
        // コマンドを作詞する関数
        void makeCommand(std::vector<std::string>& row, PLCRequestData& data,std::string& code, std::string& address);

        // デバイスコードを作成する関数
        void makeDeviceCode(std::vector<char>& buf, std::string& code);

        // 先頭デバイスナンバーを作成する関数
        void makeTopDeviceNumber(std::vector<char>& buf, std::string& address);

        // 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数
        std::array<uint8_t,4> decStrToBytes32(const std::string& decStr);

        // デバイス点数を作成する関数
        void makeDevicePoint(PLCRequestData& data, int firstNumber, int lastNumber);

        // 後ろからsubstringする関数
        std::string substrBack(std::string& str, size_t pos, size_t len);
};