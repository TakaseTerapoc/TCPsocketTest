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

    // タイムアウトの設定
    setRecvTimeout(0, 100000); // 0秒、100ミリ秒

    // ノンブロッキングモードに設定
    // setNonblocking();
}

void ServerConnectionClient::setRecvTimeout(int sec, int usec)
{
    // 受信タイムアウトの設定
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::getInstance().Error(
            fmt::format("ソケットの作成に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
        exit(1);
    }
}

void ServerConnectionClient::setSendTimeout(int sec, int usec)
{
    // 送信タイムアウトの設定
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    if (setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::getInstance().Error(
            fmt::format("ソケットの作成に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
        exit(1);
    }
}

void ServerConnectionClient::setNonblocking()
{
    // ノンブロッキングモードの設定
    int flags = fcntl(socket_, F_GETFL, 0);
    if (flags < 0) {
        Logger::getInstance().Error("ソケットのフラグ取得に失敗しました。");
        exit(1);
    }
    if (fcntl(socket_, F_SETFL, flags | O_NONBLOCK) < 0) {
        Logger::getInstance().Error("ソケットのノンブロッキングモード設定に失敗しました。");
        exit(1);
    }
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
        return false;
    }

    recvSize = static_cast<unsigned int>(receivedLen);
    return true;
}
