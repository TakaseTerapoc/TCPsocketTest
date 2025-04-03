#include <iostream> 
#include "PLCConnectionClient.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char* argv[]) {
    switch (argc)
    {
    case 1:
        std::cout << "Input PLC IPAddress And PortNumber" << std::endl;
        break;

    case 2:
        std::cout << "Input PortNumber" << std::endl;
        break;
    
    default:
        //PLCInformation plcInformation(atoi(argv[1]), atoi(argv[2]));
        std::cout << "IPAddress:" << argv[1] << " PortNumber" << argv[2]<<  std::endl;
        std::cout << "Connect" << std::endl;
        break;
    }
    spdlog::info("spdlog info");
    auto file_logger = spdlog::basic_logger_mt("basic_logger","logs/basic-log.txt");
    file_logger->info("test massage");

    

    return 0;
}