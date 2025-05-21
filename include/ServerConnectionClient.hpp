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

using namespace std;

/**
 * @brief サーバーとのUDPを管理するクラスです。
*/
class ServerConnectionClient {
public:
    ServerConnectionClient() = default;
    ServerConnectionClient(const string& serverIp, int serverPort);
    ~ServerConnectionClient();

    bool sendMessage(const string& message);

private:
    int socket_;
    struct sockaddr_in serverAddr_;
};