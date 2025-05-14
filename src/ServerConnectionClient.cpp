#include "ServerConnectionClient.hpp"

ServerConnectionClient::ServerConnectionClient(const std::string& serverIp, int serverPort) 
{
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0) {
        Logger::getInstance().Error("ソケット作成失敗しました。");
        exit(1);
    }

    std::memset(&serverAddr_, 0, sizeof(serverAddr_));
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr_.sin_addr) <= 0) {
        Logger::getInstance().Error("不正なアドレスです");
        exit(1);
    }
}

ServerConnectionClient::~ServerConnectionClient() 
{
    close(socket_);
}

bool ServerConnectionClient::sendMessage(const std::string& message) 
{
    ssize_t sentLen = sendto(socket_, message.c_str(), message.size(), 0,
                             (struct sockaddr*)&serverAddr_, sizeof(serverAddr_));
    if (sentLen < 0) {
        return false;
    }
    return true;
}
