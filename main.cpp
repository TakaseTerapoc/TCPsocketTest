#include <iostream> 
#include "PLCConnectionClient.hpp"
#include <string>
#include <thread>
#include <chrono>
// #include "spdlog/spdlog.h"
// #include "spdlog/sinks/basic_file_sink.h"

const char* MCprotocol = "500000FF03FF000018002004010000D*0002000001";
int count = 0;
char cBuff[1024];
char buf[] = {
    0x50,0x00,      // サブヘッダ
    0x00,           // ネットワーク番号（アクセス経路）
    0xFF,           // PC番号（アクセス経路）
    0xFF,0x03,      // 要求先ユニットI/O番号(アクセス経路)
    0x00,           // 要求ユニット局番号 (アクセス経路)
    0x00,0x18,      // 要求データ長
    0x20,0x00,      // 監視タイマ
    0x01,0x04,      // コマンド
    0x00,0x00,      // サブコマンド
    0x00,0x00,0x64, // 先頭デバイス番号
    0xA8,           // デバイスコード
    0x09,0x00,      // デバイス点数
};

char bufnew[] = {
    0x01,                // サブヘッダ
    0xFF,                // PC番号（アクセス経路）
    0x00,0x00,           // 監視タイマ
    0x00,0x00,0x00,0x00, // 先頭デバイス番号
    0x20,0x59,           // デバイスコード
    0x01,                // デバイス点数
    0x00         
};

void SendTask(PLCConnectionClient* sock)
{
    for(;;){
        sock->SendRequest(bufnew);
        // sock->SendRequest(reinterpret_cast<const char*>(MCprotocol));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    }
}

void RecvTask(PLCConnectionClient* sock)
{
    for(;;){
        char text[256];
        ssize_t len = sock->RecvResponse(text);
        for (int i = 0; i < len; i++)
        {
            printf("%02X ", text[i]);
        }
    }
}


int main(int argc, char* argv[]) 
{
    int PLCPort = 5021;
    const char* PLCIp = "192.168.62.201";
    PLCConnectionClient pLCConnectionClient(PLCIp, PLCPort);

    if(pLCConnectionClient.Connect() < 0)
    {
        std::cout << "接続失敗しました。"<< std::endl; 
        exit(1);
    }

    std::thread thread_send(SendTask, &pLCConnectionClient);
    // pLCConnectionClient.SendRequest(bufnew);
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