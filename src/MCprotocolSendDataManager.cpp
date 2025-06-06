#include "MCprotocolSendDataManager.hpp"
#include "Utilities.hpp"

namespace FX3UC
{
    constexpr int  CNCS32BitAddressMinValue = 200;
    constexpr int  CNCS32BitAddressMaxValue = 255;


    // シングルトンインスタンス取得
    MCprotocolSendDataManager& MCprotocolSendDataManager::getInstance() {
        static MCprotocolSendDataManager instance;
        return instance;
    }

    void MCprotocolSendDataManager::covertToMCprotocolData(vector<PLCTransactionData>& gRData)
    {
        for (auto& data : gRData)
        {
            string code;
            string address;
            int firstNumber = 0;
            int lastNumber = 0;

            // コマンドに応じてMCプロトコルを変換
            if (data.command == "read")
            {
                size_t i = 0;
                for (int i = 0; i < (data.mapdata).size(); i++)
                {
                    auto& row = data.mapdata[i];
                    if(i == 0)
                    {
                        getAddressAndCodeFromASCIIRow(row, code, address);
                        setSubheader(row, data, code, address);
                        setPCNumber(data.protocolbuf);
                        setMonitorTimer(data.protocolbuf);
                        setTopDeviceNumber(data.protocolbuf, code,address);
                        setDeviceCode(data.protocolbuf, code);
                        firstNumber = stoi(row.at("ASCII"));
                    } 
                    else if (i == (data.mapdata).size() - 1)
                    {
                        lastNumber = stoi(row.at("ASCII"));
                    }
                }
                setDevicePoint(data.protocolbuf, firstNumber, lastNumber, code, address);
                setEndPoint(data.protocolbuf);

                // 確認用
                cout << "【シリアルナンバー】" << data.serialNumber << " 【間隔】" << data.sendIntervalMs << "ミリ秒" <<endl;
                for (size_t i = 0; i < data.protocolbuf.size(); ++i) {
                    uint8_t byte = static_cast<uint8_t>(data.protocolbuf[i]);
                    printf("readPLCwithWord[%2zu] = 0x%02X\n", i, byte);
                }

            }
            else if (data.command == "write")
            {
                // data.protocolbuf = bufM00Write;
            }
            else if (data.command == "start")
            {
                // data.protocolbuf = startPLC;
            }
            else if (data.command == "stop")
            {
                // data.protocolbuf = stopPLC;
            }
            else if (data.command == "getname")
            {
                // data.protocolbuf = getPLCName;
            }
        }
    }

    void MCprotocolSendDataManager::getAddressAndCodeFromASCIIRow(map<string,string> row, string& code, string& address) {
        // ASCIIコードの最後の4文字をアドレスとして取得
        address = row.at("ASCII").substr((row.at("ASCII")).size() - ADDRESSLENGTH);
        // 残りの部分をデバイスコードとして取得
        code = row.at("ASCII").substr(0, (row.at("ASCII")).size() - ADDRESSLENGTH);
    }

    void MCprotocolSendDataManager::setSubheader(map<string,string>& row, PLCTransactionData& data, string& code, string& address)
    {
        if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("M") 
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("S")
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS")
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS")
            )
        {
            data.protocolbuf.push_back(subheader[MCprotocolConfigData::CommandEnum::READBIT]); 
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("D")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("R")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN")
        )
        {
            data.protocolbuf.push_back(subheader[MCprotocolConfigData::CommandEnum::READWORD]); 
        }
        else
        {
            Logger::getInstance().Error("setSubheader：デバイスコードが不正です。");
            exit(1);
        }

        // デバイスコードを入れる。
        data.deviceCode = code;
    }

    void MCprotocolSendDataManager::setPCNumber(vector<char>& protocolbuf) {
        // PC番号を設定
        protocolbuf.push_back(pcNumber[0]);
    }

    void MCprotocolSendDataManager::setMonitorTimer(vector<char>& protocolbuf) {
        // 監視タイマを設定
        Utilities::appendVectorElements(protocolbuf, monitorTimer);

    }

    void MCprotocolSendDataManager::setTopDeviceNumber(vector<char>& protocolbuf, string& code, string& address)
    {
        std::vector<char> bytes;
        if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")
            || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")
        )
        {
            bytes = Utilities::octStrToBytes32(address);;
        }
        else
        {
            // 10進数のアドレスを32ビットのリトルエンディアン4バイトに変換
            bytes = Utilities::decStrToBytes32(address);
        }
        // 先頭デバイス番号を設定
        Utilities::appendVectorElements(protocolbuf, bytes);
    }

    void MCprotocolSendDataManager::setDeviceCode(vector<char>& protocolbuf, string& code)
    {
        if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("M"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::M]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("D"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::D]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("X"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::X]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::Y]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::CN]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::CS]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::TN]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::TS]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("S"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::S]);
        }
        else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("R"))
        {
            Utilities::appendVectorElements(protocolbuf, MCprotocolConfigData::DeviceCodes[MCprotocolConfigData::DeviceCodeEnum::R]);
        }
        else
        {
            Logger::getInstance().Error("setTopDeviceNumber：デバイスコードが不正です。");
            exit(1);
        }
    }

    // デバイス点数を作成する関数
    void MCprotocolSendDataManager::setDevicePoint(vector<char>& protocolbuf, int firstNumber, int lastNumber, string& code, string& address)
    {
        if ( lastNumber == 0)
        {
            // デバイスコードがCNまたはCSの場合で、かつアドレス値が200-255の場合、点数を2倍にする。
            if ((code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN") )
                && ( atoi(address.c_str()) >= CNCS32BitAddressMinValue && atoi(address.c_str()) <= CNCS32BitAddressMaxValue )  
            )
            {
                protocolbuf.push_back((char)(0x01 * 2));
            }
            else
            {
                protocolbuf.push_back((char)0x01);
            }
        }
        else
        {
            int deviceNum = lastNumber - firstNumber + 1;

            // デバイスコードがCNまたはCSの場合で、かつアドレス値が200-255の場合、点数を2倍にする。
            if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN") 
                && ( atoi(address.c_str()) >= CNCS32BitAddressMinValue && atoi(address.c_str()) <= CNCS32BitAddressMaxValue )
            )
            {
                deviceNum % 2 == 0 ? deviceNum : deviceNum++; // 奇数の場合は1を足して偶数にする
                protocolbuf.push_back((char)((deviceNum) * 2 & 0xFF));
            }
            else
            {
                protocolbuf.push_back((char)((deviceNum) & 0xFF));
            }
        }
    }

    void MCprotocolSendDataManager::setEndPoint(vector<char>& protocolbuf) 
    {
        // 終点を設定
        protocolbuf.push_back((char)0x00);
    }
}