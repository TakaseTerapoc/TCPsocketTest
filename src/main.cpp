#include <iostream>
#include <string>
#include "PLCConnectionClient.hpp"
#include "PLCRequestResponseData.hpp"
#include "PLCResponseData.hpp"
#include "globals.hpp"
#include "PLCRequestScheduler.hpp"
#include "PLCRequestWorker.hpp"
#include "ResourcesManager.hpp"
#include "Logger.hpp"
#include "CSVIO.hpp"
#include "MCprotocolManager.hpp"
#include "ServerConnectionClient.hpp"
#include "ServerRequestWorker.hpp"

int main() 
{
    // Logger初期化
    Logger::getInstance().Init();

    // 設定ファイル読込み
    Logger::getInstance().Info("設定ファイルを読み込みます。");
    if (!ResourcesManager::getInstance().LoadFile("../ini/config.ini")) {
        Logger::getInstance().Error("設定ファイルの読込に失敗しました。");
        exit(1);
    }
    Logger::getInstance().Info("設定ファイルを読み込みました。");


    // PLCリクエストファイル読込
    Logger::getInstance().Info("PLCへのリクエストデータファイルを読み込みます。");
    // 【新】
    // CSVファイルをmap形式のデータに出力
    std::vector<std::map<std::string, std::string>> mapdata = CSVIO::readCSVFileToMapVector("../request/testdataDemo.csv");

    // map形式のデータをPLCRequestResponseDataに変換
    gRData = CSVIO::makeRequestDataFromMapdata(mapdata);
    for (const auto& group : gRData) {
        std::cout << "Serial: " << group.serialNumber << "\n";
        for (const auto& row : group.mapdata) {
            std::cout << "  ASCII: " << row.at("ASCII")
                    << "  device: " << row.at("device")
                    << "  sensorID: " << row.at("sensorID") << "\n";
        }
    }
    
    // 【旧】
    // try
    // {
    //     gRData = CSVIO::readCSVFile("../request/testdataDemo.csv");
    //     if(gRData.size() == 0)
    //     {
    //         Logger::getInstance().Error("PLCへのリクエストデータファイルの読込に失敗しました。");
    //         exit(1);
    //     }
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << "エラー: " << e.what() << std::endl;
    //     Logger::getInstance().Error("PLCへのリクエストデータファイルを読み込めませんでした。");
    //     exit(1);
    // }
    Logger::getInstance().Info("PLCへのリクエストデータファイルを読み込みました。");


    // MCプロトコルへ変換
    // 【新】
    MCprotocolManager::covertToMCprotocolData2(gRData);

    // 【旧】
    //MCprotocolManager::covertToMCprotocolData(gRData);


    // PLC接続テスト
    Logger::getInstance().Info("設定ファイルの情報でPLCとサーバーに接続します。");
    PLCConnectionClient pLCConnectionClient(
        ResourcesManager::getInstance().GetPLCConfig("ipaddress").c_str(), 
        atoi(ResourcesManager::getInstance().GetPLCConfig("port").c_str())
    );

    int connectionresult = -1;
    size_t i = 0;

    // PLCに接続できるまでループ
    while (connectionresult < 0)
    {
        if (i > 0)
        {
            Logger::getInstance().Error("PLCへの接続を再試行します。");
        }
        connectionresult = pLCConnectionClient.Connect();
        if(connectionresult < 0)
        {
            Logger::getInstance().Error("PLCへの接続が失敗しました。");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    Logger::getInstance().Info("PLCへの接続が成功しました。");

    //サーバー接続用意※接続はしない
    ServerConnectionClient serverConnectionClient(
        ResourcesManager::getInstance().GetServerConfig("ipaddress").c_str(), 
        atoi(ResourcesManager::getInstance().GetServerConfig("port").c_str())
    );

    // PLCスケジューラ起動
    auto& PLCScheduler = PLCRequestScheduler::getInstance();
    PLCScheduler.start();

    // PLCワーカー起動
    auto& PLCWorker = PLCRequestWorker::getInstance(pLCConnectionClient);
    PLCWorker.start();

    // サーバーワーカー起動
    auto& serverWorker = ServerRequestWorker::getInstance(serverConnectionClient);
    serverWorker.start();

    PLCWorker.join();
    PLCScheduler.join();
    serverWorker.join();

    return 0;
}