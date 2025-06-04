#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

/**
* @param MCプロトコルを設定する際の定数をまとめたデータクラスです。
*/
class MCprotocolConfigData {
    public:
        // 頭文字がDのデバイスコードの最大読取ワード数
        static constexpr int WORD_MAX_READ_SIZE = 4;

        // 頭文字がM・X・Y・S・T・Cのデバイスコードの最大読取点数(bit読み取り時)
        static constexpr int BIT_MAX_READ_SIZE = 200; //なぜか12までしか読めない

        inline static const map<string,string> deviceCodeToASCIIMap = {
            {"CN", "6778"},
            {"CS", "6783"},
            {"D", "68"},
            {"M", "77"},
            {"R", "82"},
            {"S", "83"},
            {"TN", "8478"},
            {"TS", "8483"},
            {"X", "88"},
            {"Y", "89"}
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