#include "ServerConnectionClient.hpp"

ServerConnectionClient::ServerConnectionClient(const string& serverIp, int serverPort) 
{
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0) {
        Logger::getInstance().Error("ソケット作成失敗しました。");
        exit(1);
    }

    memset(&serverAddr_, 0, sizeof(serverAddr_));
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

bool ServerConnectionClient::sendMessage(const char* message, unsigned int messageSize) 
{
    if (message == nullptr || messageSize == 0) {
        Logger::getInstance().Error("送信メッセージが無効です。");
        return false;
    }

    // メッセージを送信
    ssize_t sentLen = sendto(socket_, message, messageSize, 0,
                             (struct sockaddr*)&serverAddr_, sizeof(serverAddr_));
    if (sentLen < 0) return false;
    return true;
}

bool ServerConnectionClient::recvMessage(char* buffer, unsigned int bufferSize, unsigned int& recvSize) 
{
    if (buffer == nullptr || bufferSize == 0) {
        Logger::getInstance().Error("受信バッファが無効です。");
        return false;
    }

    // メッセージを受信
    socklen_t addrLen = sizeof(serverAddr_);
    ssize_t receivedLen = recvfrom(socket_, buffer, bufferSize, 0,
                                   (struct sockaddr*)&serverAddr_, &addrLen);
    if (receivedLen < 0) {
        Logger::getInstance().Error("メッセージの受信に失敗しました。");
        return false;
    }

    recvSize = static_cast<unsigned int>(receivedLen);
    return true;
}
