# include "PLCConnectionClient.hpp"

PLCConnectionClient::PLCConnectionClient(const char* serverIpAddress, int serverPortNumber)
{
    this.serverAddress_.sin_addr.s_addr = inet_addr(serverIpAddress);
    this.serverAddress_.sin_port = htons(serverPortNumber);
    this.serverAddress_.sin_family = AF_INET; // AF_INET：IPV4アドレスを使う
}