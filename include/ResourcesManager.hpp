#pragma once

#include "../external/SimpleIni/SimpleIni.h"
#include <string>

class ResourcesManager {
public:
    // 唯一のインスタンスを取得
    static ResourcesManager& getInstance();

    // 設定ファイル読み込み
    bool LoadFile(const std::string& filename);

    // PLCConfig セクションから値を取得
    std::string GetPLCConfig(const std::string& key);

    // serverConfig セクションから値を取得
    std::string GetServerConfig(const std::string& key);

private:
    ResourcesManager();
    ~ResourcesManager();

    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;

    CSimpleIniA ini;
};
