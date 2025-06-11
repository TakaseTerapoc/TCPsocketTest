#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logger.hpp"
#include "../external/fmt/format.h"

const int TimeoutSec = 0; // タイムアウト秒数
const int TimeoutUsec = 500000; // タイムアウトマイクロ秒数

using namespace std;

class PLCConnectionClient
{
    private:
        int socket_;
        int sockAddressLen_ = sizeof(struct sockaddr_in);
        struct sockaddr_in serverAddress_;
        
    public:
        // コンストラクタとデストラクタ
        PLCConnectionClient() = default;
        ~PLCConnectionClient() { 
            Logger::getInstance().Debug("PLCConnectionClientのデストラクタが呼ばれました。ソケットを開放します。");
            Close(); 
        }

        // コピー禁止
        PLCConnectionClient(const PLCConnectionClient&) = delete;
        PLCConnectionClient& operator=(const PLCConnectionClient&) = delete;


        PLCConnectionClient(const char* serverIpAddress, int serverPortNumber);

        int makeSocket();
        void setTimeout(int sec, int usec);
        int Connect();
        void Close();
        void getConnInfo(const char* serverIpAddress, int serverPortNumber);
        int sendRequest(const char* text, int len, int& sendLen);
        int recvResponse(char* text, int textSize, int& recvLen);
        

        struct timeval timeout;
};
