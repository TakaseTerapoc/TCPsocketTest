#include "MCprotocolManager.hpp"
#include "MCprotocolConfig.hpp"

vector<char> readPLCwithBit = 
{
    (char)0x00,                                     // サブヘッダ 0番目
    (char)0xff,                                     // PC番号（アクセス経路）1番目
    (char)0x00,(char)0x00,                          // 監視タイマ 2番目3番目
    (char)0x0a,(char)0x00,(char)0x00,(char)0x00,    // 先頭デバイス番号 4番目5番目6番目7番目
    (char)0x00,(char)0x00,                          // デバイスコード 8番目9番目
    (char)0x00,                                     // デバイス点数 10番目
    (char)0x00,                                     // 終点 11番目
};

vector<char> readPLCwithWord = 
{
    (char)0x01,                                     // サブヘッダ 0番目
    (char)0xff,                                     // PC番号（アクセス経路）1番目
    (char)0x00,(char)0x00,                          // 監視タイマ 2番目3番目
    (char)0x0a,(char)0x00,(char)0x00,(char)0x00,    // 先頭デバイス番号 4番目5番目6番目7番目
    (char)0x00,(char)0x00,                          // デバイスコード 8番目9番目
    (char)0x00,                                     // デバイス点数 10番目
    (char)0x00,                                     // 終点 11番目
};


vector<char> startPLC = 
{
    (char)0x13,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ
    (char)0x00,
};

vector<char> stopPLC = 
{
    (char)0x14,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ      
    (char)0x00,
};


vector<char> getPLCName = 
{
    (char)0x15,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ
    (char)0x00,
};

vector<char> test = 
{
    (char)0x16,                                     // サブヘッダ
    (char)0xFF,                                     // PC番号（アクセス経路）
    (char)0x00,(char)0x00,                          // 監視タイマ
    (char)0x03,(char)0x01,(char)0x02,(char)0x03     // テストデータ
};

void MCprotocolManager::covertToMCprotocolData2(vector<PLCTransactionData>& gRData)
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
                // data.deviceCount++;
                auto& row = data.mapdata[i];
                if(i == 0)
                {
                    makeCommand2(row, data, code, address);
                    makeTopDeviceNumber(data.protocolbuf, address);
                    makeDeviceCode(data.protocolbuf, code);
                    firstNumber = stoi(row.at("ASCII"));
                } 
                else if (i == (data.mapdata).size() - 1)
                {
                    lastNumber = stoi(row.at("ASCII"));
                }
            }
            makeDevicePoint(data, firstNumber, lastNumber);

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
            data.protocolbuf = startPLC;
        }
        else if (data.command == "stop")
        {
            data.protocolbuf = stopPLC;
        }
        else if (data.command == "getname")
        {
            data.protocolbuf = getPLCName;
        }
    }
}

void MCprotocolManager::makeCommand2(map<string,string>& row, PLCTransactionData& data, string& code, string& address)
{
    address = row.at("ASCII").substr((row.at("ASCII")).size() - 4);
    code = row.at("ASCII").substr(0, (row.at("ASCII")).size() - 4);

    if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("M") 
        || code == MCprotocolConfig::deviceCodeToASCIIMap.at("X")
        || code == MCprotocolConfig::deviceCodeToASCIIMap.at("Y")
        || code == MCprotocolConfig::deviceCodeToASCIIMap.at("S")
        || code == MCprotocolConfig::deviceCodeToASCIIMap.at("TS")
        || code == MCprotocolConfig::deviceCodeToASCIIMap.at("CS")
        )
    {
        data.protocolbuf = readPLCwithBit;
        makeDeviceCode(data.protocolbuf, code);
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("D")
    || code == MCprotocolConfig::deviceCodeToASCIIMap.at("R")
    || code == MCprotocolConfig::deviceCodeToASCIIMap.at("TN")
    || code == MCprotocolConfig::deviceCodeToASCIIMap.at("CN")
    )
    {
        data.protocolbuf = readPLCwithWord;
        makeDeviceCode(data.protocolbuf, code);
    }
    else
    {
        Logger::getInstance().Error("デバイスコードが不正です。");
        exit(1);
    }

    // デバイスコードを入れる。
    data.deviceCode = code;
}

vector<map<string,string>> MCprotocolManager::convertResponseDataToSendData2(char* text, int len, PLCTransactionData& req) {
    string responseData;
    string format;
    vector<map<string,string>> sendData;

    for (int i = 0; i < len; ++i)
    {
        if (i > 1)
        {
            responseData += convertBytesToHexString(text[i]);

        }
        else
        {
            format += convertBytesToHexString(text[i]);
        }
    }

    Logger::getInstance().Info("【シリアルナンバー】" + req.serialNumber + "【フォーマット】"+ format +"【受信データ(16進数)】" + responseData);

    for (int i = 0; i < req.mapdata.size(); i++)
    {
        string data;
        if (req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("M") 
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("X")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("Y")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("S")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("TS")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("CS")
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
        else if (req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("D")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("R")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("TN")
            || req.deviceCode == MCprotocolConfig::deviceCodeToASCIIMap.at("CN")
        )
        {
            if (i == 0)
            {
                data = convertDecimalString(swapString(responseData.substr(0, 4)));

            }
            else
            {
                int startPosition = (stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"])) * 4;
                data = convertDecimalString(swapString(responseData.substr(startPosition, 4)));
            }
        }
        req.mapdata[i]["data"] = data;
    }
    sendData = req.mapdata;

    return sendData;
}

string MCprotocolManager::convertBytesToHexString(char& bytes) {
    
    return fmt::format("{:02X}", static_cast<unsigned char>(bytes));
}

string MCprotocolManager::substrBack(string& str, size_t pos, size_t len) {
    const size_t strLen = str.length();

    return str.substr(strLen - pos, len);
}

void MCprotocolManager::makeDeviceCode(vector<char>& buf, string& code)
{
    if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("M"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x4D;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("D"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x44;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("X"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x58;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("Y"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x59;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("CN"))
    {
        buf[8] = (char)0x4E;
        buf[9] = (char)0x43;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("CS"))
    {
        buf[8] = (char)0x53;
        buf[9] = (char)0x43;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("TN"))
    {
        buf[8] = (char)0x4E;
        buf[9] = (char)0x54;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("TS"))
    {
        buf[8] = (char)0x53;
        buf[9] = (char)0x54;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("S"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x53;
    }
    else if (code == MCprotocolConfig::deviceCodeToASCIIMap.at("R"))
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x52;
    }
    else
    {
        Logger::getInstance().Error("デバイスコードが不正です。");
        exit(1);
    }
}

void MCprotocolManager::makeTopDeviceNumber(vector<char>& buf, string& address)
{
    auto bytes = decStrToBytes32(address);
    for (int i = 0; i < 4; ++i) {
        buf[4 + i] = bytes[i];
    }
}

// 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数      
array<uint8_t,4> MCprotocolManager::decStrToBytes32(const string& decStr) 
{
    unsigned long v = stoul(decStr, nullptr, 10);
    return {
        static_cast<uint8_t>( v        & 0xFF),
        static_cast<uint8_t>((v >>  8) & 0xFF),
        static_cast<uint8_t>((v >> 16) & 0xFF),
        static_cast<uint8_t>((v >> 24) & 0xFF)
    };
}

// デバイス点数を作成する関数
void MCprotocolManager::makeDevicePoint(PLCTransactionData& data, int firstNumber, int lastNumber)
{
    if ( lastNumber == 0)
    {
        data.protocolbuf[10] = (char)0x01;
    }
    else
    {
        data.protocolbuf[10] = (char)((lastNumber - firstNumber + 1) & 0xFF);
    }
}

string MCprotocolManager::swapString(const string& str) {
    string first = str.substr(0, 2);
    string second = str.substr(2, 2);
    string result = second + first;
    return result; 
}

string MCprotocolManager::convertDecimalString(const string& hex) {
    return to_string(strtoul(hex.c_str(), NULL, 16));
}