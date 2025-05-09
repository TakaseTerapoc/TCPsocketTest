#include "PLCRequestData.hpp"

std::string READ = "read";
std::string WRITE = "write";

void PLCRequestData::convertMCprotocol()
{
    std::string commandstr = this->command;
    std::string dataAddressstr = this->dataAddress;
    if (commandstr == "getname")
    {
        protocolbuf[0] = static_cast<char>(0x15);
        protocolbuf[0] = static_cast<char>(0x15);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
    }
    else if (commandstr == "start")
    {
        protocolbuf.push_back(0x13);
        protocolbuf.push_back(0xff);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
    }
    else if (commandstr == "stop")
    {
        protocolbuf.push_back(0x13);
        protocolbuf.push_back(0xff);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
        protocolbuf.push_back(0x00);
    }

    if (commandstr == "read")
    {
        char fs = firstChar(dataAddressstr);
        if (fs == 'D' || fs == 'R' || fs == 'T' || fs == 'C')
        {
            protocolbuf.push_back(0x01);
            protocolbuf.push_back(0xff);
            protocolbuf.push_back(0x00);
            protocolbuf.push_back(0x00);
        }

    }
}

char PLCRequestData::firstChar(const std::string& s) {
    if (s.empty()) {
        std::cout << "文字列が空です" << std::endl;
        exit(1);
    }
    return s[0];
}