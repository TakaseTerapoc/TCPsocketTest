#pragma once

#include <vector>
/**
* @param MCプロトコルを設定する際の定数をまとめたデータクラスです。
*/
#include <map>
#include <string>

using namespace std;

class MCprotocolConfig {
    public:
        // 頭文字がDのデバイスコードの最大読取ワード数
        static constexpr int D_WORD_MAX_READ_SIZE = 4;

        // 頭文字がM・X・Y・S・T・Cのデバイスコードの最大読取点数(bit読み取り時)
        static constexpr int M_X_Y_S_T_C_BIT_MAX_READ_SIZE = 12; 

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

        // static const enum DeviceCodeASCII {
        //     C = 67,
        //     D = 68,
        //     M = 77,
        //     R = 82,
        //     S = 83,
        //     T = 84,
        //     X = 88,
        //     Y = 89
        // };
};