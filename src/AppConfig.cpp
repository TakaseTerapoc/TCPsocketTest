#include "AppConfig.hpp"
#include "FileResources.hpp"
#include "Logger.hpp"

// シングルトン取得
AppConfig& AppConfig::getInstance() {
    static AppConfig instance;
    return instance;
}

bool AppConfig::LoadFiles(const string& filename) {
    // パスファイルを読み込む
    if (!LoadFile(filename, filePathIni)) 
    {
        Logger::getInstance().Error("パスファイルの読み込みに失敗しました。");
        return false;
    }
    configFilePath = GetString(filePathIni, FileResources::FILEPATH, FileResources::CONFIGFILEPATH);
    requestFilePath = GetString(filePathIni, FileResources::FILEPATH, FileResources::REQUESTFILEPATH);

    // 設定ファイルを読み込む
    if (!LoadFile(configFilePath, ini)) 
    {
        Logger::getInstance().Error("設定ファイルの読み込みに失敗しました。");
        return false;
    }
    return true;
}

// 設定ファイルの読み込み
bool AppConfig::LoadFile(const string& filename, CSimpleIniA& anyIni) {
    SI_Error rc = anyIni.LoadFile(filename.c_str());
    return rc >= 0;
}

string AppConfig::GetString(CSimpleIniA& anyIni, const string& title, const string& key) {
    // iniファイルから文字列を取得
    const char* pVal = anyIni.GetValue(title.c_str(), key.c_str(), "undefined");
    return string(pVal);
}

// PLCConfig セクションから文字列を取得
string AppConfig::GetPLCConfig(const string& key) {
    const char* pVal = ini.GetValue("PLCConfig", key.c_str(), "undefined");
    return string(pVal);
}

// serverConfig セクションから文字列を取得
string AppConfig::GetServerConfig(const string& key) {
    const char* pVal = ini.GetValue("serverConfig", key.c_str(), "undefined");
    return string(pVal);
}
