#pragma once

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class PLCConnectionClient
{
private:
    int socket_;
    int sockAddressLen_ = sizeof(struct sockaddr_in);
    struct sockaddr_in serverAddress_;
public:
    PLCConnectionClient(){}
    PLCConnectionClient(const char* serverIpAddress, int serverPortNumber);

    // 遅延初期化のための仕組み
    inline PLCConnectionClient& getPLCConnectionClient()
    {
        static PLCConnectionClient instance;
        return instance;
    }

    int Connect();
    void getConnInfo(const char* serverIpAddress, int serverPortNumber);
    void SendRequest(const char* text, int len);
    ssize_t RecvResponse();
};
