#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logger.hpp"
#include "../external/fmt/format.h"

const int TimeoutSec = 5; // タイムアウト秒数
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

    int Connect();
    void getConnInfo(const char* serverIpAddress, int serverPortNumber);
    void SendRequest(const char* text, int len);
    int RecvResponse(char* text);

    struct timeval timeout;
};
