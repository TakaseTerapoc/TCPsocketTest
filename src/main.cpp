#include <iostream>
#include "PLCConnectionClient.hpp"
#include "PLCRequestData.hpp"
#include "PLCResponseData.hpp"
#include "globals.hpp"
#include "PLCRequestScheduler.hpp"
#include "PLCRequestWorker.hpp"
#include "ResourcesManager.hpp"
#include "Logger.hpp"

int main() 
{
    // Logger初期化
    Logger::getInstance().Init();

    // 設定ファイル読込み
    Logger::getInstance().Debug("設定ファイルを読み込みます。");
    if (!ResourcesManager::getInstance().LoadFile("../ini/config.ini")) {
        Logger::getInstance().Error("設定ファイルの読込に失敗しました。");
        exit(1);
    }
    Logger::getInstance().Info("設定ファイルを読み込みました。");

    // PLC接続テスト
    Logger::getInstance().Info("設定ファイルの情報でPLCとサーバーに接続します。");
    PLCConnectionClient pLCConnectionClient(
        ResourcesManager::getInstance().GetPLCConfig("ipaddress").c_str(), 
        atoi(ResourcesManager::getInstance().GetPLCConfig("port").c_str())
    );

    if(pLCConnectionClient.Connect() < 0)
    {
        Logger::getInstance().Error("PLCへの接続が失敗しました。");
        exit(1);
    }
    else
    {
        Logger::getInstance().Info("PLCへの接続が成功しました。");
    }


    // PLCリクエストファイル読込
    Logger::getInstance().Info("PLCへのリクエストデータファイルを読み込みます。");
    // try
    // {
    //     boost::property_tree::read_json(RequestDataFile, pt);
    // }
    // catch(const boost::property_tree::json_parser_error& e)
    // {
    //     std::cout << "リクエストデータファイルが読み込めませんでした。"<< std::endl;
    //     exit(1);
    // }
    Logger::getInstance().Info("PLCへのリクエストデータファイルを読み込みました。");


    // PLCリクエストファイルの内容を該当クラスに格納
    for (int i = 0; i < 2; i++)
    {
        PLCRequestData pLCRequestData
        (
            "1",
            "read",
            "D100",
            1,
            1000,
            ResourcesManager::getInstance().GetPLCConfig("ipaddress").c_str(), 
            atoi(ResourcesManager::getInstance().GetPLCConfig("port").c_str()),
            ResourcesManager::getInstance().GetServerConfig("ipaddress").c_str(), 
            atoi(ResourcesManager::getInstance().GetServerConfig("port").c_str())
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
    auto& worker = PLCRequestWorker::getInstance(pLCConnectionClient);
    worker.start();

    worker.join();
    scheduler.join();

    return 0;
}