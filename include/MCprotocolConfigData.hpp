#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

/**
* @param MCプロトコルを設定する際の定数をまとめたデータクラスです。
*/

namespace FX3UC
{
    class MCprotocolConfigData {
        private:
            // コンストラクタとデストラクタを削除してインスタンス化を防ぐ
            MCprotocolConfigData() = delete;
            ~MCprotocolConfigData() = delete; 
        public:
            // 頭文字がDのデバイスコードの最大読取ワード数
            static constexpr int WORD_MAX_READ_SIZE = 4;

            // 頭文字がM・X・Y・S・T・Cのデバイスコードの最大読取点数(bit読み取り時)
            static constexpr int BIT_MAX_READ_SIZE = 200;

            inline static const map<string,string> deviceCodeToASCIIMap = {
                {"X", "88"},
                {"Y", "89"},
                {"M", "77"},
                {"S", "83"},
                {"TS", "8483"},
                {"CS", "6783"},
                {"D", "68"},
                {"R", "82"},
                {"TN", "8478"},
                {"CN", "6778"},
            };

            inline static const vector<int> DeviceAddressMaxSize = {
                377, // X
                377, // Y
                8511, // M
                4095, // S
                511, // TS
                255, // CS
                8511, // D
                32767, // R
                511, // TN
                255, // CN
            };

            inline static const vector<vector<char>> DeviceCodes = {
                {(char)0x20, (char)0x58}, // X
                {(char)0x20, (char)0x59}, // Y
                {(char)0x20, (char)0x4D}, // M
                {(char)0x20, (char)0x53}, // S
                {(char)0x53, (char)0x54}, // TS
                {(char)0x53, (char)0x43}, // CS
                {(char)0x20, (char)0x44}, // D
                {(char)0x20, (char)0x52}, // R
                {(char)0x4E, (char)0x54}, // TN
                {(char)0x4E, (char)0x43}  // CN
            };

            enum DeviceCodeEnum{
                X = 0,
                Y = 1,
                M = 2,
                S = 3,
                TS = 4,
                CS = 5,
                D = 6,
                R = 7,
                TN = 8,
                CN = 9,
            };

            enum CommandEnum{
                READBIT = 0,          // ビット一括読み出し
                READWORD = 1,         // ワード一括読み出し
                WRITEBIT = 2,         // ビット一括書き込み
                WRITEWORD = 3,        // ワード一括書き込み
                WRITERANDOMBIT = 4,   // ビットランダム書き込み
                WRITERANDOMWORD = 5,  // ワードランダム書き込み
            };

            // サブヘッダの配列
            inline static vector<char> subheader = 
            {
                (char)0x00,                             // ビット一括読み出し
                (char)0x01,                             // ワード一括読み出し
                (char)0x02,                             // ビット一括書き込み
                (char)0x03,                             // ワード一括書き込み
                (char)0x04,                             // ビットランダム書き込み
                (char)0x05,                             // ワードランダム書き込み
            };

            // PC番号の配列
            inline static vector<char> pcNumber = 
            {
                (char)0xff,                                     // PC番号（固定）
            };

            // 監視タイマの配列
            inline static vector<char> monitorTimer = 
            {
                (char)0x00, (char)0x00                          // 監視タイマ（固定:無制限）
            };

            // 先頭デバイス番号の配列
            inline static vector<char> topDeviceNumber = 
            {
                (char)0x00, (char)0x00, (char)0x00, (char)0x00  // 先頭デバイス番号(初期値)
            };        

            // デバイスコードの配列
            inline static vector<char> deviceCode = 
            {
                (char)0x00, (char)0x00                          // デバイスコード（初期値）
            };

            // デバイス点数の配列
            inline static vector<char> devicePoint = 
            {
                (char)0x00                                      // デバイス点数（初期値）
            };

            // 終点の配列
            inline static vector<char> endPoint = 
            {
                (char)0x00                                      // 終点（固定）
            };
    };
}