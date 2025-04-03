#ifndef PLCINFORMATION_HPP
#define PLCINFORMATION_HPP

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class PLCConnectionClient
{
private:
    int socket_;
    int sockAddressLen_ = sizeof(struct sockaddr_in);
    struct sockaddr_in serverAddress_;
public:
    PLCConnectionClient(const char* serverIpAddress, int serverPortNumber);
};

#endif
