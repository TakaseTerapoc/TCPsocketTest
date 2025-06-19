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
 * @brief サーバーとのUDP通信を管理するクラスです。
*/
class ServerConnectionClient {
public:
    // コンストラクタとデストラクタ
    ServerConnectionClient() = default;
    ServerConnectionClient(const string& serverIp, int serverPort);
    ~ServerConnectionClient()
    {
        if (socket_ >= 0) {
            close(socket_);
            Logger::getInstance().Debug("サーバー接続クライアントのソケットを閉じました。");
        }
    };

    // サーバーへメッセージを送信する関数
    bool sendMessage(const char* message, unsigned int messageSize);

    // サーバーからメッセージを受信する関数
    bool recvMessage(char* buffer, unsigned int bufferSize, unsigned int& recvSize);

private:
    int socket_;
    struct sockaddr_in serverAddr_;

    // ノンブロッキングモードを設定する関数
    void setNonblocking();

    // 受信タイムアウトを設定する関数
    void setRecvTimeout(int sec, int usec);

    // 送信タイムアウトを設定する関数
    void setSendTimeout(int sec, int usec);

    struct timeval timeout;
};