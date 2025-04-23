#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <fstream> 
#include "PLCConnectionClient.hpp"
#include "PLCRequestData.hpp"
#include "PLCData.hpp"
#include "globals.hpp"
// #include "PLCRequestScheduler.hpp"
#include "Config.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::placeholders; 

// #include "spdlog/spdlog.h"
// #include "spdlog/sinks/basic_file_sink.h"

const std::string ConfigFile = "config/config.json";
const std::string RequestDataFile = "config/requestdata.json";
const char* PLCSettingsIPAddress = "PLCSettings.IPAddress";
const char* PLCSettingsPort = "PLCSettings.Port";

std::queue<PLCRequestData> requestQueue;

bool isResponded = true;

int count = 0;
char cBuff[256];
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

char bufTest[] = {
    0x16,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00,           // 監視タイマ
    0x03,0x01,0x02,0x03 // テストデータ
};

const char bufD8400[] = {
    0x01,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x0a,0x00,           // 監視タイマ
    0xD0,0x20,0x00,0x00, // 先頭デバイス番号
    0x20,0x44,           // デバイスコード
    0x02                // デバイス点数 
};

const char bufD100[] = {
    0x01,
    0xff,
    0x00,0x00,
    0x64,0x00,0x00,0x00,
    0x20,0x44,
    0x01,
    0x00
};

char bufM10[] = {
    0x00,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00,           // 監視タイマ
    0x0a,0x00,0x00,0x00, // 先頭デバイス番号
    0x20,0x4D,           // デバイスコード
    0x02,                // デバイス点数
    0x00, 
};

char bufM00Write[] = {
    0x03,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00,           // 監視タイマ
    0x64,0x00,0x00,0x00, // 先頭デバイス番号
    0x20,0x4D,           // デバイスコード
    0x01,                // デバイス点数 
    0x00,
};

char bufStart[] = {
    0x13,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00           // 監視タイマ      
};

char bufStop[] = {
    0x14,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00           // 監視タイマ      
};


char bufName[] = {
    0x15,                // サブヘッダ
    0xff,                // PC番号（アクセス経路）
    0x00,0x00,
    0x00,          // 監視タイマ    
};

void SendTask(PLCConnectionClient* sock)
{
    for(;;){
        sock->SendRequest(bufM10, sizeof(bufM10));
        isResponded = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
}

void RecvTask(PLCConnectionClient* sock)
{
    for(;;){
        char text[256];
        ssize_t len = sock->RecvResponse(text);
        if (len > 0){
            for (int i = 0; i < len; ++i)
            {
                printf("%02X ", text[i]);
            }
            printf("\n");
        }
    }
}


int main(int argc, char* argv[]) 
{
    std::cout << "設定ファイルを読み込みます。"<< std::endl; 
    
    // 設定ファイル読込み
    boost::property_tree::ptree pt;
    try
    {
        boost::property_tree::read_json(ConfigFile, pt);
    }
    catch(const boost::property_tree::json_parser_error& e)
    {
        std::cout << "設定ファイルが読み込めませんでした。"<< std::endl;
        exit(1);
    }
    std::cout << "設定ファイルを読み込みました。"<< std::endl;
    Config config(
        pt.get<std::string>("PLCSettings.IPAddress"),
        pt.get<int>("PLCSettings.Port"),
        pt.get<std::string>("ServerSettings.IPAddress"),
        pt.get<int>("ServerSettings.Port")
    );

    // PLC接続テスト
    std::cout << "設定ファイルの情報でPLCとサーバーに接続します。"<< std::endl; 
    PLCConnectionClient pLCConnectionTest(
        config.getPLCIpAddress().c_str(), 
        config.getPLCPortNumber()
    );

    if(pLCConnectionTest.Connect() < 0)
    {
        std::cout << "PLCに接続失敗しました。"<< std::endl; 
        exit(1);
    }
    else
    {
        std::cout << "PLCに接続成功しました。"<< std::endl; 
    }


    // PLCリクエストファイル読込
    std::cout << "PLCへのリクエストデータファイルを読み込みます。"<< std::endl; 
    try
    {
        boost::property_tree::read_json(RequestDataFile, pt);
    }
    catch(const boost::property_tree::json_parser_error& e)
    {
        std::cout << "リクエストデータファイルが読み込めませんでした。"<< std::endl;
        exit(1);
    }
    std::cout << "リクエストデータファイルを読み込みました。"<< std::endl;


    // PLCリクエストファイルの内容を該当クラスに格納
    std::vector<PLCRequestData> rdata;

    for (const auto& item : pt.get_child("devices"))
    {
        PLCRequestData pLCRequestData
        (
            item.second.get<std::string>("serialNumber"),
            item.second.get<std::string>("command"),
            item.second.get<std::string>("dataAddress"),
            item.second.get<int>("deviceCount"),
            item.second.get<int>("sendIntervalMs"),
            config.getPLCIpAddress().c_str(), 
            config.getPLCPortNumber(),
            config.getserverIpAddress().c_str(), 
            config.getserverPortNumber()
        );
        rdata.push_back(pLCRequestData);
    }

    for (const auto& r : rdata) {
        std::cout << r.command << " @ " << r.dataAddress << "\n";
    }

    // スケジューラ起動

    // std::thread thread_send(SendTask, &pLCConnectionClient);
    // std::thread thread_recv(RecvTask, &pLCConnectionClient);
    // thread_send.join();
    // thread_recv.join();

    return 0;
}