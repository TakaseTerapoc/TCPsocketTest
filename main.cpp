#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <fstream> 
#include "PLCConnectionClient.hpp"
#include "PLCRequestData.hpp"
#include "PLCResponseData.hpp"
#include "globals.hpp"
#include "PLCRequestScheduler.hpp"
#include "PLCRequestWorker.hpp"
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
    gPLCClient.getPLCConnectionClient().getConnInfo(config.getPLCIpAddress().c_str(), config.getPLCPortNumber());
    // PLCConnectionClient pLCConnectionClient(
    //     config.getPLCIpAddress().c_str(), 
    //     config.getPLCPortNumber()
    // );

    if(gPLCClient.Connect() < 0)
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
        gRData.push_back(pLCRequestData);
    }

    for (const auto& r : gRData) {
        std::cout << r.command << " @ " << r.dataAddress << "\n";
    }

    
    // スケジューラ起動
    auto& scheduler = PLCRequestScheduler::getInstance();
    scheduler.start();

    // ワーカー起動
    auto& worker = PLCRequestWorker::getInstance();
    worker.start();

    // std::queue<PLCRequestData> tmp = requestQueue;
    // while (true) {
    //     if(requestQueue.empty())
    //     {
    //         continue;
    //     }
    //     auto req = requestQueue.front();
    //     // requestQueue.pop();
    //     auto dur = req.nextTime.time_since_epoch();
    //     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    //     std::cout << "device=" << req.dataAddress
    //                 << "time=" << ms
    //                 << ", interval="   << req.sendIntervalMs << "\n";
    // }
    // while(true){}
    worker.join();
    scheduler.join();
    // std::thread thread_send(SendTask, &pLCConnectionClient);
    // std::thread thread_recv(RecvTask, &pLCConnectionClient);
    // thread_send.join();
    // thread_recv.join();

    return 0;
}