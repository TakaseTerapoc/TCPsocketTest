# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    serverAddress_.sin_port = htons(serverPortNumber);
    serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}

void PLCConnectionClient::Connect()
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    connect(socket_, (sockaddr *)&serverAddress_, sockAddressLen_);
}

void PLCConnectionClient::SendRequest(const char* text)
{
    send(socket_, text, std::strlen(text), 0);
}

void PLCConnectionClient::RecvResponse(char* text)
{
    ssize_t recvSize = recv(socket_, text, 1024, 0);
    text[recvSize] = '\0';
}