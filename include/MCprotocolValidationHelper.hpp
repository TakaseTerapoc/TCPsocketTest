#pragma once

#include <vector>
#include <map>
#include <string>

#include "Logger.hpp"
#include "MCprotocolConfigData.hpp"

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

            // デバイスコードのアドレスが最大サイズを超えているかチェックする関数
            inline bool checkDeviceAddressMaxSize (const string& deviceCode, const string& address) 
            {
                int value = atoi(address.c_str());

                if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")) {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::X];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::Y];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("M")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::M];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("S")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::S];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::TS];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::CS];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("D")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::D];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("R")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::R];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::TN];
                } else if (deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN")) 
                {
                    return value <= MCprotocolConfigData::DeviceAddressMaxSize[MCprotocolConfigData::DeviceCodeEnum::CN];
                }
                return false;
            };
        private:
    };
}