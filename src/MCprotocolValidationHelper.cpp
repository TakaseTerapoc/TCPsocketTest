#include "MCprotocolValidationHelper.hpp"

bool MCprotocolValidationHelper::checkValidation(string& deviceASCII)
{
    string initialDevice = Utilities::substrCustom(deviceASCII, ADDRESSLENGTH); 
    string deviceAddress = Utilities::substrBack(deviceASCII, ADDRESSLENGTH); 

    // デバイスコードのアドレスが最大サイズを超えていないかチェックする
    if (!checkDeviceAddressMaxSize(initialDevice, deviceAddress)) {
        Logger::getInstance().Error("アドレスコードの最大値を超えています。");
        return false;
    }

    // Mアドレスの読み取り禁止リストに含まれているかチェックする
    if (checkprohibitedReadingAddress(stoi(deviceAddress))) {
        Logger::getInstance().Error("読み取りが禁止されているアドレスが含まれています。");
        return false;
    }

    return true;
}

bool MCprotocolValidationHelper::checkOddNumber(int devicePoint)
{
    // デバイスコードのアドレスが奇数かどうかチェックする
    if (devicePoint % 2 != 0) {
        return false;
    }
    return true;
}

// Mアドレスの読み取り禁止リストに含まれているかチェックする関数
bool MCprotocolValidationHelper::checkprohibitedReadingAddress(int address) 
{   
    return find(MCprotocolConfigData::prohibitedReadingMaddress.begin(), 
                MCprotocolConfigData::prohibitedReadingMaddress.end(), address) != MCprotocolConfigData::prohibitedReadingMaddress.end();
};

// デバイスコードのアドレスが最大サイズを超えているかチェックする関数
bool MCprotocolValidationHelper::checkDeviceAddressMaxSize (const string& deviceCode, const string& address) 
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