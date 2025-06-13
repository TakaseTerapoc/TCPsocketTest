#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm> 
#include <array>

#include "Logger.hpp"

using namespace std;

/**
 * @brief ユーティリティ関数をまとめたクラスです。
*/
class Utilities
{
    public:
        // コンストラクタ デストラクタ
        Utilities() = delete; // インスタンス化を防ぐためにコンストラクタを削除
        ~Utilities() = delete;

        // vector<map<string, string>>を文字列に変換する関数
        // 例:"{key1: value1, key2: value2}, {key3: value3}"
        static string convertVectorMapToString(const vector<map<string, string>>& m);

        // バイト値のvectorを16進数の文字列に変換する関数
        static string convertVectorBytesToHexString(const vector<char>& bytes);

        // バイト値を16進数の文字列に変換する関数
        static string convertByteToHexString(char& bytes);

        // 既存のvector配列に新しいvectorを追加する関数
        template<typename T>
        static vector<T> appendVectorElements(vector<T>& dest, const vector<T>& src)
        {
            dest.insert(dest.end(), src.begin(), src.end());
            return dest;
        }

        // 16進数の文字列を10進数の文字列に変換する関数
        static std::string convertDecimalString(const std::string& hex);

        // 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数
        static vector<char> decStrToBytes32ByLittleEndian(const string& decStr);

        // 8進数の文字列を32ビットのリトルエンディアン4バイトに分解する関数
        static vector<char> octStrToBytes32ByLittleEndian(const string& octStr);

        // 指定した位置でsubstrする関数
        static string substrCustom(std::string& str, size_t len);

        // 後ろからsubstringする関数
        static string substrBack(std::string& str, size_t len);

        // 16Bitのレスポンス文字列を正しい順番に並び替える。
        static string swapString16Bit(const std::string& str);

        // 16Bitのレスポンス文字列を正しい順番に並び替える。
        static string swapString32Bit(const std::string& str);

        // map<string, string>からキーの値をint型で取得する関数
        static int getMapValueByInt(const map<string, string>& row, string keystr);

        // map<string, string>からキーの値をstring型で取得する関数
        static string getMapValueByString(const map<string, string>& row, string keystr);

        // 10進数を16ビットのビッグエンディアン2バイトに変換する関数
        static vector<char> decToBytes16ByBigEndian(unsigned int dec);

        // stringをvector<char>に変換する関数
        static vector<char> convertStringToVectorChar(const string& str);

        // vector<string>をstringに変換する関数
        static string convertVectorStringToString(const vector<string>& sendDataVector);
};