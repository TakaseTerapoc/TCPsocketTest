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
#include "PLCRequestData.hpp"


class MCprotocolManager
{
    public:
        //コンストラクタ
        MCprotocolManager() = delete;

        // PLCRequestDataの各種メンバーからMCプロトコルデータに変換する関数
        static void covertToMCprotocolData(std::vector<PLCRequestResponseData>& gRData);

        // コマンドを作詞する関数
        static void makeCommand(std::vector<std::string>& row, PLCRequestResponseData& data,std::string& code, std::string& address);

        // デバイスコードを作成する関数
        static void makeDeviceCode(std::vector<char>& buf, std::string& code);

        // 先頭デバイスナンバーを作成する関数
        static void makeTopDeviceNumber(std::vector<char>& buf, std::string& address);

        // 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数
        static std::array<uint8_t,4> decStrToBytes32(const std::string& decStr);

        // デバイス点数を作成する関数
        static void makeDevicePoint(PLCRequestResponseData& data, int firstNumber, int lastNumber);

        // 後ろからsubstringする関数
        static std::string substrBack(std::string& str, size_t pos, size_t len);

        // レスポンスデータ(１６進数)を10進数に変換して算出する関数
        static std::string convertSendData(char* text, int len, PLCRequestResponseData& req);

        // 16進→2進数文字列変換
        static std::string toBinaryString(uint8_t byte); 

        // 送信データを作成する関数
        static void makeSendData(PLCRequestResponseData& data, std::string& code, std::string& address, int firstNumber, int lastNumber);
};