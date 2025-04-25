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
    std::cout << "PLCに接続します。sock="<< socket_ << std::endl; 
    result = connect(socket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_));
    return result;
}

void PLCConnectionClient::SendRequest(const char* text, int len)
{
    std::cout << "PLCにリクエストを送ります。"<< std::endl; 
    std::cout << len << "::" << socket_<<std::endl; 
    for (int i = 0; i < len; ++i)
    {
        printf("%02X ", text[i]);
    }
    printf("\n");

    // if (send(socket_, text, len, 0) < 0)
    // {
    //     std::cout << "送信エラー"<< std::endl; 
    // };
    try {
        std::cout << "[run] → SendR 前\n";
        send(socket_, text, len, 0);
        std::cout << "[run] ← Send 後\n";
    } catch (const std::exception& e) {
        std::cout << "[run][ERROR] Send 例外: " << e.what() << "\n";
    }
}

ssize_t PLCConnectionClient::RecvResponse()
{
    std::cout << "受信開始します。"<< std::endl; 
    char text[256];
    ssize_t recvSize = recv(socket_, text, sizeof(text), 0);
    std::cout << "受信しました。" << recvSize << std::endl; 
    return recvSize;
}