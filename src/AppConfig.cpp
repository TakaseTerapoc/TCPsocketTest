#include "AppConfig.hpp"

// コンストラクタ／デストラクタ
AppConfig::AppConfig() = default;
AppConfig::~AppConfig() = default;

// シングルトン取得
AppConfig& AppConfig::getInstance() {
    static AppConfig instance;
    return instance;
}

// 設定ファイルの読み込み
bool AppConfig::LoadFile(const string& filename) {
    SI_Error rc = ini.LoadFile(filename.c_str());
    return rc >= 0;
}

// PLCConfig セクションから文字列を取得
string AppConfig::GetPLCConfig(const string& key) {
    const char* pVal = ini.GetValue("PLCConfig", key.c_str(), "（未定義）");
    return string(pVal);
}

// serverConfig セクションから文字列を取得
string AppConfig::GetServerConfig(const string& key) {
    const char* pVal = ini.GetValue("serverConfig", key.c_str(), "（未定義）");
    return string(pVal);
}
