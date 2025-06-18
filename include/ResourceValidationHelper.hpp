#pragma once

#include <vector>
#include <map>
#include <string>
#include <regex>

#include "Logger.hpp"

#define MIN_PORT 1
#define MAX_PORT 65535


using namespace std;

/**
 * @brief リソースのバリデーションをチェックするヘルパークラスです。
 */
class ResourceValidationHelper {
    public:
        // コンストラクタ デストラクタ
        ResourceValidationHelper() = default;
        ~ResourceValidationHelper() = default;

        // リソースのバリデーションチェック関数
        inline static bool checkPortValidation(const string& portStr)
        {
            try{
                int port = stoi(portStr);
                if (port < MIN_PORT || port > MAX_PORT) {
                    Logger::getInstance().Error("ポート番号は1から65535の範囲でなければなりません。");
                    return false;
                }
            } catch (const out_of_range& e) {
                Logger::getInstance().Error("ポート番号が不正です: " + string(e.what()));
                return false;
            } catch (const invalid_argument& e) {
                Logger::getInstance().Error("ポート番号が整数ではありません: " + string(e.what()));
                return false;
            }
            return true;
        }

        // Ipアドレスのバリデーションチェック関数
        inline static bool checkIpAddressValidation(const string& ipAddress)
        {
            // IPアドレスの形式をチェックする正規表現
            const regex ipRegex(
                R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))"
            );
            if (!regex_match(ipAddress, ipRegex)) {
                Logger::getInstance().Error("IPアドレスの形式が不正です: " + ipAddress);
                return false;
            }
            return true;
        }

    private:
};