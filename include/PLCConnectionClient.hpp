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

    int Connect();
    void getConnInfo(const char* serverIpAddress, int serverPortNumber);
    void SendRequest(const char* text, int len);
    ssize_t RecvResponse(char* text);
};
