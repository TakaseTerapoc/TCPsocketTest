#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm> 

#include "Logger.hpp"

using namespace std;

/**
 * @brief ユーティリティ関数をまとめたクラスです。
*/
class Utilities
{
    public:
        // コンストラクタ
        Utilities() = delete; // インスタンス化を防ぐためにコンストラクタを削除

        // vector<map<string, string>>を文字列に変換する関数
        // 例:"{key1: value1, key2: value2}, {key3: value3}"
        static string convertVectorMapToString(const vector<map<string, string>>& m);

        // バイト値のvectorを16進数の文字列に変換する関数
        static string convertBytesToHexString(const vector<char>& bytes);



};