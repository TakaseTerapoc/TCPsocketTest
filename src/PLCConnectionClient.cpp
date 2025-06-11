# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う

    // ソケットの作成
    socket_ = makeSocket();

    // タイムアウトの設定
    setTimeout(TimeoutSec, TimeoutUsec); 
}

void PLCConnectionClient::getConnInfo(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}

int PLCConnectionClient::makeSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    {
        Logger::getInstance().Error(
            fmt::format("ソケットの作成に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
        exit(1);
    }
    return sock;
}

void PLCConnectionClient::setTimeout(int sec, int usec)
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

    // 送信タイムアウトの設定
    if (setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::getInstance().Error(
            fmt::format("ソケットの作成に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
        exit(1);
    }
}

int PLCConnectionClient::Connect()
{
    int result = connect(socket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_));
    if (result < 0) {
        Logger::getInstance().Error(
            fmt::format("PLCへの接続に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
        return -1; // 接続失敗
    }
    return result;
}

void PLCConnectionClient::Close()
{
    if (socket_ != -1)
    {
        close(socket_);
        Logger::getInstance().Debug("ソケットを閉じました。");
        socket_ = -1; // ソケットを閉じた後は無効化
    }
}

int PLCConnectionClient::sendRequest(const char* text, int len, int& sendLen)
{
    Logger::getInstance().Info("送信開始します。"); 
    sendLen = send(socket_, text, len, 0);
    if (sendLen < 0) {
        Logger::getInstance().Error(
            fmt::format("送信に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
    }
    return sendLen;
}

int PLCConnectionClient::recvResponse(char* text, int textSize, int& recvLen)
{
    Logger::getInstance().Info("受信開始します。"); 
    recvLen = recv(socket_, text, textSize, 0);
    if (recvLen < 0) {
        Logger::getInstance().Error(
            fmt::format("受信に失敗しました。errno={}, message={}", errno, strerror(errno))
        );
    }
    return recvLen;
}

