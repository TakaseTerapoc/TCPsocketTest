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
    try
    {
        gRData = CSVIO::readCSVFile("../request/testdataDemo.csv");
        if(gRData.size() == 0)
        {
            Logger::getInstance().Error("PLCへのリクエストデータファイルの読込に失敗しました。");
            exit(1);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "エラー: " << e.what() << std::endl;
        Logger::getInstance().Error("PLCへのリクエストデータファイルを読み込めませんでした。");
        exit(1);
    }
    Logger::getInstance().Info("PLCへのリクエストデータファイルを読み込みました。");

    // MCプロトコルへ変換
    MCprotocolManager::covertToMCprotocolData(gRData);

    // // TODO:後で消す
    // return 0;

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