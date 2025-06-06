#pragma once

#include <vector>
#include <map>
#include <string>

#include "Logger.hpp"
#include "MCprotocolConfigData.hpp"
#include "Utilities.hpp"
#include "globals.hpp"

using namespace std;

/**
* @param MCプロトコルのバリデーションをチェックするクラスです。
*/
namespace FX3UC
{
    class MCprotocolValidationHelper {
        public:
            // コンストラクタ デストラクタ
            MCprotocolValidationHelper() = default;
            ~MCprotocolValidationHelper() = default;
            
            // このクラスの全てのバリデーションチェック関数を呼び出す関数
            bool checkValidation(string& deviceASCII);

            // 奇数かチェックする
            bool checkOddNumber(int devicePoint);

        private:
        
            // Mアドレスの読み取り禁止リストに含まれているかチェックする関数
            bool checkprohibitedReadingAddress(int address);

            // デバイスコードのアドレスが最大サイズを超えているかチェックする関数
            bool checkDeviceAddressMaxSize (const string& deviceCode, const string& address);
    };
}