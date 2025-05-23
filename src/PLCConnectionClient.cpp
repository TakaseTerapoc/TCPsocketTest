# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う

    socket_ = makeSocket();
    if (socket_ < 0) 
    {
        Logger::getInstance().Error("ソケット作成に失敗しました。");
    }
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
        Logger::getInstance().Error("ソケット作成に失敗しました。");
        exit(1);
    }
    return sock;
}

int PLCConnectionClient::Connect()
{
    int result = connect(socket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_));
    return result;
}

int PLCConnectionClient::close()
{
    int result = ::close(socket_);
    return result;
}

int PLCConnectionClient::sendRequest(const char* text, int len, int& sendLen)
{
    Logger::getInstance().Info("送信開始します。"); 
    sendLen = send(socket_, text, len, 0);
    return sendLen;
}

int PLCConnectionClient::recvResponse(char* text, int& recvLen)
{
    Logger::getInstance().Info("受信開始します。"); 
    recvLen = recv(socket_, text, sizeof(text), 0);
    return recvLen;
}

