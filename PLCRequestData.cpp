#include "PLCRequestData.hpp"

PLCRequestData::PLCRequestData(){}

PLCRequestData::PLCRequestData(
    std::string serialNumber, 
    std::string command, 
    std::string dataAddress, 
    int deviceCount, 
    int sendIntervalMs,
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
    this->sendIntervalMs = sendIntervalMs;
    this->PLCIpAddress = PLCIpAddress;
    this->PLCPortNumber = PLCPortNumber;
    this->serverIpAddress = serverIpAddress;
    this->serverPortNumber = serverPortNumber;
}
