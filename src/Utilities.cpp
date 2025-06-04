#include "Utilities.hpp"

string Utilities::convertVectorMapToString(const vector<map<string, string>>& vectorMap)
{
    string sendDatastr;
    for (size_t i = 0; i < vectorMap.size(); ++i) {
        sendDatastr += "{";
            // 各mapのキーと値を文字列に変換
        for (const auto& [key, value] : vectorMap[i]) {
                sendDatastr += key + ": " + value + ", ";
            }
        sendDatastr += "}, ";
    }
    return sendDatastr;
}

string Utilities::convertVectorBytesToHexString(const vector<char>& bytes)
{
    string hexStr;
    for (const auto& byte : bytes) {
        hexStr += fmt::format("{:02x}", byte);
    }
    return hexStr;
}

// バイト値を16進数の文字列に変換する関数
string Utilities::convertByteToHexString(char& bytes) 
{
    
    return fmt::format("{:02X}", static_cast<unsigned char>(bytes));
}

string Utilities::convertDecimalString(const string& hex) {
    return to_string(strtoul(hex.c_str(), NULL, 16));
}

// 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数      
vector<char> Utilities::decStrToBytes32(const string& decStr) 
{
    unsigned long v = stoul(decStr, nullptr, 10);
    return {
        static_cast<char>( v        & 0xFF),
        static_cast<char>((v >>  8) & 0xFF),
        static_cast<char>((v >> 16) & 0xFF),
        static_cast<char>((v >> 24) & 0xFF)
    };
}

// 後ろからsubstringする関数
// posは後ろからの位置、lenは取得する文字数
string Utilities::substrBack(string& str, size_t pos, size_t len) 
{
    const size_t strLen = str.length();

    return str.substr(strLen - pos, len);
}

// レスポンス文字列を正しい順番に並び替える関数
string Utilities::swapString(const string& str) {
    string first = str.substr(0, 2);
    string second = str.substr(2, 2);
    string result = second + first;
    return result; 
}

// TODO:数値に変換できない時の例外処理
int Utilities::getMapValueByInt(const map<string, string>& row, string keystr) 
{

    auto it = row.find(keystr);
    if (it != row.end()) {
        return stoi(it->second);
    }
    else {
        Logger::getInstance().Error("キーが見つかりません: " + keystr);
        exit(1); // エラー時はアプリケーションを終了
    }
}

// TODO:数値に変換できない時の例外処理
string Utilities::getMapValueByString(const map<string, string>& row, string keystr) 
{

    auto it = row.find(keystr);
    if (it != row.end()) {
        return it->second;
    }
    else {
        Logger::getInstance().Error("キーが見つかりません: " + keystr);
        exit(1); // エラー時はアプリケーションを終了
    }
}