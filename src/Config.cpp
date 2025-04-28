
#include "Config.hpp"


const std::string ConfigFile = "config/config.json";

Config::Config(std::string pLCIpAddress, int pLCPortNumber, std::string serverIpAddress, int serverPortNumber)
{
    this->PLCIpAddress = pLCIpAddress;
    this->PLCPortNumber = pLCPortNumber;
    this->serverIpAddress = serverIpAddress;
    this->serverPortNumber = serverPortNumber;
}

/**
 * @brief 設定ファイルを読込み各メンバーに格納する関数です。
 */
void Config::readConfigFile()
{
    std::cout << "設定ファイルを読み込みます。"<< std::endl; 

    //設定ファイル読込
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

    // 各メンバーに格納
    PLCIpAddress = pt.get<std::string>("PLCSettings.IPAddress");
    PLCPortNumber = pt.get<int>("PLCSettings.Port");
    serverIpAddress = pt.get<std::string>("ServerSettings.IPAddress");
    serverPortNumber = pt.get<int>("ServerSettings.Port");
}