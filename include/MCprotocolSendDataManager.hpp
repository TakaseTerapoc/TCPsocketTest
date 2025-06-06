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
    class MCprotocolSendDataManager
    {
        public:
            // シングルトンインスタンスを取得する関数
            static MCprotocolSendDataManager& getInstance();

            // PLCTransactionDataを受け取り、MCプロトコルのデータを作成する関数
            void covertToMCprotocolData(std::vector<PLCTransactionData>& gRData);

        private:
            // コンストラクタとデストラクタ
            MCprotocolSendDataManager() = default;
            ~MCprotocolSendDataManager() = default;

            // コピー禁止
            MCprotocolSendDataManager(const MCprotocolSendDataManager&) = delete;
            MCprotocolSendDataManager& operator=(const MCprotocolSendDataManager&) = delete;

            
            // ASCIIコードの行からアドレスとデバイスコードを取得する関数
            void getAddressAndCodeFromASCIIRow(map<string,string> row, string& code, string& address); 

            // サブヘッダを作成する関数
            void setSubheader(std::map<std::string,std::string>& row, PLCTransactionData& data, std::string& code, std::string& address);

            // PC番号を設定する関数
            void setPCNumber(std::vector<char>& protocolbuf);

            // 監視タイマを設定する関数
            void setMonitorTimer(std::vector<char>& protocolbuf);

            // デバイスコードを作成する関数（例：D100のD）
            void setDeviceCode(std::vector<char>& protocolbuf, std::string& code);

            // 先頭デバイスナンバーを作成する関数
            void setTopDeviceNumber(std::vector<char>& protocolbuf, std::string& address);

            // デバイス点数を作成する関数
            void setDevicePoint(vector<char>& protocolbuf, int firstNumber, int lastNumber, string& code, string& address);

            //終点を設定する関数
            void setEndPoint(vector<char>& protocolbuf);

            // サブヘッダの配列
            const vector<char> subheader = 
            {
                (char)0x00,                             // ビット一括読み出し
                (char)0x01,                             // ワード一括読み出し
                (char)0x02,                             // ビット一括書き込み
                (char)0x03,                             // ワード一括書き込み
                (char)0x04,                             // ビットランダム書き込み
                (char)0x05,                             // ワードランダム書き込み
            };

            // PC番号の配列
            const vector<char> pcNumber = 
            {
                (char)0xff,                                     // PC番号（固定）
            };

            // 監視タイマの配列
            const vector<char> monitorTimer = 
            {
                (char)0x00, (char)0x00,                          // 監視タイマ（固定:無制限）
            };

            // 先頭デバイス番号の配列
            vector<char> topDeviceNumber = 
            {
                (char)0x00, (char)0x00, (char)0x00, (char)0x00,  // 先頭デバイス番号(初期値)
            };        

            // デバイスコードの配列
            vector<char> deviceCode = 
            {
                (char)0x00, (char)0x00,                          // デバイスコード（初期値）
            };

            // デバイス点数の配列
            vector<char> devicePoint = 
            {
                (char)0x00,                                      // デバイス点数（初期値）
            };

            // 終点の配列
            const vector<char> endPoint = 
            {
                (char)0x00,                                      // 終点（固定）
            };
    };
}