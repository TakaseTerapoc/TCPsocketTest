#include "MCprotocolManager.hpp"
#include "MCprotocolConfigData.hpp"
#include "Utilities.hpp"

// シングルトンインスタンス取得
MCprotocolManager& MCprotocolManager::getInstance() {
    static MCprotocolManager instance;
    return instance;
}

void MCprotocolManager::covertToMCprotocolData(vector<PLCTransactionData>& gRData)
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
                    setTopDeviceNumber(data.protocolbuf, address);
                    setDeviceCode(data.protocolbuf, code);
                    firstNumber = stoi(row.at("ASCII"));
                } 
                else if (i == (data.mapdata).size() - 1)
                {
                    lastNumber = stoi(row.at("ASCII"));
                }
            }
            setDevicePoint(data.protocolbuf, firstNumber, lastNumber);
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

void MCprotocolManager::getAddressAndCodeFromASCIIRow(map<string,string> row, string& code, string& address) {
    // ASCIIコードの最後の4文字をアドレスとして取得
    address = row.at("ASCII").substr((row.at("ASCII")).size() - ADDRESSLENGTH);
    // 残りの部分をデバイスコードとして取得
    code = row.at("ASCII").substr(0, (row.at("ASCII")).size() - ADDRESSLENGTH);
}

void MCprotocolManager::setSubheader(map<string,string>& row, PLCTransactionData& data, string& code, string& address)
{
    if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("M") 
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("S")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS")
        || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS")
        )
    {
        data.protocolbuf.push_back(subheader[CommandEnum::READBIT]); 
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("D")
    || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("R")
    || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN")
    || code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN")
    )
    {
        data.protocolbuf.push_back(subheader[CommandEnum::READWORD]); 
    }
    else
    {
        Logger::getInstance().Error("setSubheader：デバイスコードが不正です。");
        exit(1);
    }

    // デバイスコードを入れる。
    data.deviceCode = code;
}

void MCprotocolManager::setPCNumber(vector<char>& protocolbuf) {
    // PC番号を設定
    protocolbuf.push_back(pcNumber[0]);
}

void MCprotocolManager::setMonitorTimer(vector<char>& protocolbuf) {
    // 監視タイマを設定
    Utilities::appendVectorElements(protocolbuf, monitorTimer);

}

void MCprotocolManager::setTopDeviceNumber(vector<char>& protocolbuf, string& address)
{
    // 10進数のアドレスを32ビットのリトルエンディアン4バイトに変換
    auto bytes = Utilities::decStrToBytes32(address);
    // 先頭デバイス番号を設定
    Utilities::appendVectorElements(protocolbuf, bytes);
}

void MCprotocolManager::setDeviceCode(vector<char>& protocolbuf, string& code)
{
    if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("M"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::M]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("D"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::D]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("X"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::X]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::Y]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::CN]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::CS]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::TN]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::TS]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("S"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::S]);
    }
    else if (code == MCprotocolConfigData::deviceCodeToASCIIMap.at("R"))
    {
        Utilities::appendVectorElements(protocolbuf, DeviceCodes[DeviceCodeEnum::R]);
    }
    else
    {
        Logger::getInstance().Error("setTopDeviceNumber：デバイスコードが不正です。");
        exit(1);
    }
}

// デバイス点数を作成する関数
void MCprotocolManager::setDevicePoint(vector<char>& protocolbuf, int firstNumber, int lastNumber)
{
    if ( lastNumber == 0)
    {
        protocolbuf.push_back((char)0x01);
    }
    else
    {
        protocolbuf.push_back((char)((lastNumber - firstNumber + 1) & 0xFF));
    }
}

void MCprotocolManager::setEndPoint(vector<char>& protocolbuf) 
{
    // 終点を設定
    protocolbuf.push_back((char)0x00);
}

vector<map<string,string>> MCprotocolManager::convertResponseDataToSendData(char* text, int len, PLCTransactionData& req) {
    string responseData;
    string format;
    vector<map<string,string>> sendData;

    for (int i = 0; i < len; ++i)
    {
        if (i > 1)
        {
            responseData += Utilities::convertByteToHexString(text[i]);

        }
        else
        {
            format += Utilities::convertByteToHexString(text[i]);
        }
    }

    Logger::getInstance().Info("【シリアルナンバー】" + req.serialNumber + "【フォーマット】"+ format +"【受信データ(16進数)】" + responseData);

    for (int i = 0; i < req.mapdata.size(); i++)
    {
        string data;
        if (req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("M") 
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("S")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS")
        )
        {
            if (i == 0)
            {
                data = responseData.substr(0, 1);
            }
            else
            {
                int startPosition = stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"]);
                data = responseData.substr(startPosition, 1);
            }
        }
        else if (req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("D")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("R")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN")
            || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN")
        )
        {
            if (i == 0)
            {
                data = Utilities::convertDecimalString(Utilities::swapString(responseData.substr(0, 4)));

            }
            else
            {
                int startPosition = (stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"])) * 4;
                data = Utilities::convertDecimalString(Utilities::swapString(responseData.substr(startPosition, 4)));
            }
        }
        req.mapdata[i]["data"] = data;
    }
    sendData = req.mapdata;

    return sendData;
}