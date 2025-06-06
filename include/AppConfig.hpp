#pragma once

#include "../external/SimpleIni/SimpleIni.h"
#include <string>

using namespace std;

class AppConfig {
public:
    // 唯一のインスタンスを取得
    static AppConfig& getInstance();

    // 各種設定ファイルの読み込み
    bool LoadFiles(const std::string& filename);

    // 設定ファイル読み込み
    bool LoadFile(const std::string& filename, CSimpleIniA& anyIni);

    // iniファイルから文字列を取得
    string GetString(CSimpleIniA& anyIni, const string& title, const string& key);

    // PLCConfig セクションから値を取得
    std::string GetPLCConfig(const std::string& key);

    // serverConfig セクションから値を取得
    std::string GetServerConfig(const std::string& key);

    // 設定ファイルのパス
    string configFilePath = "";
    // リクエストファイルのパス
    string requestFilePath = "";

private:
    AppConfig() = default;
    ~AppConfig() = default;

    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    CSimpleIniA ini;
    CSimpleIniA filePathIni;


};
