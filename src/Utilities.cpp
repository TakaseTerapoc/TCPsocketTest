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

string Utilities::convertBytesToHexString(const vector<char>& bytes)
{
    string hexStr;
    for (const auto& byte : bytes) {
        hexStr += fmt::format("{:02x}", byte);
    }
    return hexStr;
}
