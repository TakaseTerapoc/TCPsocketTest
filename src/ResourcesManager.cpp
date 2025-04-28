#include "ResourcesManeger.hpp"

    bool ResourcesManager::LoadMessages(const std::string& filename)
    {
        SI_Error rc = ini.LoadFile(filename.c_str());
        return rc >= 0;
    }

    std::string ResourcesManager::GetLogMessages(const std::string& key)
    {
        const char* pVal = ini.GetValue("LogMessages", key.c_str(), "（未定義）");
        return std::string(pVal);
    }

    std::string ResourcesManager::GetConfigInformation(const std::string& key)
    {
        const char* pVal = ini.GetValue("ConfigInformation", key.c_str(), "（未定義）");
        return std::string(pVal);
    }