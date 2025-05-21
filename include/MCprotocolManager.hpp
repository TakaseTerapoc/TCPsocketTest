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
#include "PLCRequestResponseData.hpp"

using namespace std;

class MCprotocolManager
{
    public:
        //コンストラクタ
        MCprotocolManager() = delete;

        //【新】MCプロトコルデータを変換する関数
        static void covertToMCprotocolData2(std::vector<PLCRequestResponseData>& gRData);

        static void makeCommand2(std::map<std::string,std::string>& row, PLCRequestResponseData& data, std::string& code, std::string& address);

        static std::vector<std::map<std::string,std::string>> convertResponseDataToSendData2(char* text, int len, PLCRequestResponseData& req);


        //【旧】MCプロトコルデータを変換する関数
        // PLCRequestDataの各種メンバーからMCプロトコルデータに変換する関数
        static void covertToMCprotocolData(std::vector<PLCRequestResponseData>& gRData);

        // コマンドを作成する関数
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
        static std::vector<std::vector<std::string>> convertResponseDataToSendData(char* text, int len, PLCRequestResponseData& req);

        // レスポンス文字列を正しい順番に並び替える。
        static std::string swapString(const std::string& str);

        // 16進→10進文字列変換
        static std::string convertDecimalString(const std::string& hex);
};