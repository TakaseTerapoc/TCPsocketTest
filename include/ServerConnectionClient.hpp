#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>

#include "Logger.hpp"

/**
 * @brief サーバーとのUDPを管理するクラスです。
*/
class ServerConnectionClient {
public:
    ServerConnectionClient() = default;
    ServerConnectionClient(const std::string& serverIp, int serverPort);
    ~ServerConnectionClient();

    bool sendMessage(const std::string& message);

private:
    int socket_;
    struct sockaddr_in serverAddr_;
};