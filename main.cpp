#include <iostream> 
#include "PLCConnectionClient.hpp"
#include <string>
#include <thread>
#include <chrono>
// #include "spdlog/spdlog.h"
// #include "spdlog/sinks/basic_file_sink.h"

const char* MCprotocol = "500000FF03FF000018002004010000D*0002000001";
int count = 0;

void SendTask(PLCConnectionClient* sock)
{
    for(;;){
        std::string text = "cilent->server" + std::to_string(count);
        count++;
        sock->SendRequest(text.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
}

void RecvTask(PLCConnectionClient* sock)
{
    for(;;){
        char text[1024];
        sock->RecvResponse(text);
        std::cout << "Recive : " << text << std::endl; 
    }
}


int main(int argc, char* argv[]) 
{
    int PLCPort = 5021;
    const char* PLCIp = "192.168.62.201";

    PLCConnectionClient pLCConnectionClient(PLCIp, PLCPort);
    pLCConnectionClient.Connect();

    std::thread thread_send(SendTask, &pLCConnectionClient);
    std::thread thread_recv(RecvTask, &pLCConnectionClient);
    thread_send.join();
    thread_recv.join();
    // PLCConnectionClient.SendRequest(MCprotocol);
    // PLCConnectionClient.RecvResponse(text);

    // switch (argc)
    // {
    // case 1:
    //     std::cout << "Input PLC IPAddress And PortNumber" << std::endl;
    //     break;

    // case 2:
    //     std::cout << "Input PortNumber" << std::endl;
    //     break;
    
    // default:
    //     //PLCInformation plcInformation(atoi(argv[1]), atoi(argv[2]));
    //     std::cout << "IPAddress:" << argv[1] << " PortNumber" << argv[2]<<  std::endl;
    //     std::cout << "Connect" << std::endl;
    //     break;
    // }
    // spdlog::info("spdlog info");
    // auto file_logger = spdlog::basic_logger_mt("basic_logger","logs/basic-log.txt");
    // file_logger->info("test massage");

    

    return 0;
}