# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}

void PLCConnectionClient::getConnInfo(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}

int PLCConnectionClient::Connect()
{
    int result;

    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    result = connect(socket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_));
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

ssize_t PLCConnectionClient::RecvResponse(char* text)
{
    Logger::getInstance().Info("受信開始します。"); 
    ssize_t recvSize = recv(socket_, text, sizeof(text), 0);
    Logger::getInstance().Info("受信しました。"+ std::to_string(recvSize) + "byte");
    return recvSize;
}