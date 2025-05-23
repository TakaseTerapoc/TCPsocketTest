# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う

    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) 
    {
        Logger::getInstance().Error("ソケット作成に失敗しました。");
    }
    
    // ソケットのオプション設定
    // 受信タイムアウトの設定
    timeout.tv_sec = TimeoutSec;
    timeout.tv_usec = TimeoutUsec;
    if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::getInstance().Error("ソケット作成に失敗しました。");
        close(socket_);
    }

    // 送信タイムアウトの設定
    if (setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::getInstance().Error("ソケット作成に失敗しました。");
        close(socket_);
    }
}

void PLCConnectionClient::getConnInfo(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}

int PLCConnectionClient::Connect()
{
    int result = connect(socket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_));
    return result;
}

void PLCConnectionClient::SendRequest(const char* text, int len)
{
    Logger::getInstance().Info("PLCにリクエストを送ります。");
    for (int i = 0; i < len; ++i)
    {
        printf("%02X ", text[i]);
    }
    printf("\n");

    if (send(socket_, text, len, 0) < 0)
    {
        Logger::getInstance().Error("PLCへのリクエスト送信に失敗しました。");
    };
}

int PLCConnectionClient::RecvResponse(char* text)
{
    Logger::getInstance().Info("受信開始します。"); 
    int recvSize = recv(socket_, text, sizeof(text), 0);
    Logger::getInstance().Info("受信しました。"+ std::to_string(recvSize) + "byte");
    return recvSize;
}