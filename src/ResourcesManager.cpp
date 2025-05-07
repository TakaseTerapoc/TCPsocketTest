#include "ResourcesManager.hpp"

// コンストラクタ／デストラクタ
ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

// シングルトン取得
ResourcesManager& ResourcesManager::getInstance() {
    static ResourcesManager instance;
    return instance;
}

// 設定ファイルの読み込み
bool ResourcesManager::LoadFile(const std::string& filename) {
    SI_Error rc = ini.LoadFile(filename.c_str());
    return rc >= 0;
}

// PLCConfig セクションから文字列を取得
std::string ResourcesManager::GetPLCConfig(const std::string& key) {
    const char* pVal = ini.GetValue("PLCConfig", key.c_str(), "（未定義）");
    return std::string(pVal);
}

// serverConfig セクションから文字列を取得
std::string ResourcesManager::GetServerConfig(const std::string& key) {
    const char* pVal = ini.GetValue("serverConfig", key.c_str(), "（未定義）");
    return std::string(pVal);
}
