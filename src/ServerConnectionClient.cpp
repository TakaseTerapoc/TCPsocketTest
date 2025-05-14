// UDPClient.cpp
#include "ServerConnectionClient.hpp"

UDPClient::UDPClient(const std::string& serverIp, int serverPort) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }
}

UDPClient::~UDPClient() {
    close(sockfd);
}

bool UDPClient::sendMessage(const std::string& message) {
    ssize_t sentLen = sendto(sockfd, message.c_str(), message.size(), 0,
                             (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sentLen < 0) {
        perror("sendto failed");
        return false;
    }
    return true;
}
