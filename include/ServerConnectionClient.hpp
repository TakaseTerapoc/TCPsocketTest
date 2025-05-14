#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>


class UDPClient {
public:
    UDPClient(const std::string& serverIp, int serverPort);
    ~UDPClient();

    bool sendMessage(const std::string& message);

private:
    int sockfd;
    struct sockaddr_in serverAddr;
};