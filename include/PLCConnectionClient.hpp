#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logger.hpp"
#include "../external/fmt/format.h"

const int TimeoutSec = 1; // タイムアウト秒数
const int TimeoutUsec = 0; // タイムアウトマイクロ秒数

using namespace std;

class PLCConnectionClient
{
private:
    int socket_;
    int sockAddressLen_ = sizeof(struct sockaddr_in);
    struct sockaddr_in serverAddress_;
public:
    PLCConnectionClient(){}
    PLCConnectionClient(const char* serverIpAddress, int serverPortNumber);

    int makeSocket();
    int Connect();
    int close();
    void getConnInfo(const char* serverIpAddress, int serverPortNumber);
    int sendRequest(const char* text, int len, int& sendLen);
    int recvResponse(char* text, int& recvLen);
    

    struct timeval timeout;
};
