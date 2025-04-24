#include "PLCRequestData.hpp"

PLCRequestData::PLCRequestData(){}

PLCRequestData::PLCRequestData(
    std::string serialNumber, 
    std::string command, 
    std::string dataAddress, 
    int deviceCount, 
    int transmissionIntervalMs,
    std::string PLCIpAddress,
    int PLCPortNumber,
    std::string serverIpAddress,
    int serverPortNumber
    )
{
    this->serialNumber = serialNumber;
    this->command = command;
    this->dataAddress = dataAddress;
    this->deviceCount = deviceCount;
    this->transmissionIntervalMs = transmissionIntervalMs;
    this->PLCIpAddress = PLCIpAddress;
    this->PLCPortNumber = PLCPortNumber;
    this->serverIpAddress = serverIpAddress;
    this->serverPortNumber = serverPortNumber;
    this->MCprotocol[0] = 0x01;
    this->MCprotocol[1] = 0xff;
    this->MCprotocol[2] = 0x00;
    this->MCprotocol[3] = 0x00;
    this->MCprotocol[4] = 0x64;
    this->MCprotocol[5] = 0x00;
    this->MCprotocol[6] = 0x00;
    this->MCprotocol[7] = 0x00;
    this->MCprotocol[8] = 0x20;
    this->MCprotocol[9] = 0x44;
    this->MCprotocol[10] = 0x01;
    this->MCprotocol[11] = 0x00;
}
