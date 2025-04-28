#pragma once

#include "SimpleIni.h"

#include <iostream>
#include <string>

class ResourcesManager
{
public:
    bool LoadMessages(const std::string& filename);

    std::string GetLogMessages(const std::string& key);

    std::string GetConfigInformation(const std::string& key);

private:
    CSimpleIniA ini;
};