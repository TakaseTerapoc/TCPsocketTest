#include "MCprotocolManager.hpp"

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

    if (code == "77" || code == "88")
    {
        data.protocolbuf = readPLCwithBit;
        makeDeviceCode(data.protocolbuf, code);
    }
    else if (code == "68")
    {
        data.protocolbuf = readPLCwithWord;
        makeDeviceCode(data.protocolbuf, code);
    }
    else
    {
        // TODO:後で他のコマンドも追加する。
        // data.protocolbuf = bufD100;
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
            responseData += convertBytesCharToString(text[i]);

        }
        else
        {
            format += convertBytesCharToString(text[i]);
        }
    }

    Logger::getInstance().Info("【シリアルナンバー】" + req.serialNumber + "【フォーマット】"+ format +"【受信データ(16進数)】" + responseData);

    for (int i = 0; i < req.mapdata.size(); i++)
    {
        string data;
        if (req.deviceCode == "77")
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
        else if (req.deviceCode == "68")
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

string MCprotocolManager::convertBytesCharToString(char& bytes) {
    
    return fmt::format("{:02X}", static_cast<unsigned char>(bytes));
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
            string code;
            string address;
            for (int i = 0; i < (data.sensorrows).size(); i++)
            {
                // data.deviceCount++;
                auto& row = data.sensorrows[i];
                if(i == 0)
                {
                    makeCommand(row, data, code, address);
                    makeTopDeviceNumber(data.protocolbuf, address);
                    makeDeviceCode(data.protocolbuf, code);
                    firstNumber = stoi(row[2]);
                } 
                else if (i == (data.sensorrows).size() - 1)
                {
                    lastNumber = stoi(row[2]);
                }
            }
            makeDevicePoint(data, firstNumber, lastNumber);

            // 確認用
            cout << "【シリアルナンバー】" << data.serialNumber << endl;
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

string MCprotocolManager::substrBack(string& str, size_t pos, size_t len) {
    const size_t strLen = str.length();

    return str.substr(strLen - pos, len);
}
void MCprotocolManager::makeCommand(vector<string>& row, PLCTransactionData& data, string& code, string& address)
{
    address = row[2].substr(row[2].size() - 4);
    code = row[2].substr(0, row[2].size() - 4);
    if (code == "77" || code == "88")
    {
        data.sendIntervalMs = 60000;
        data.protocolbuf = readPLCwithBit;
        makeDeviceCode(data.protocolbuf, code);
    }
    else if (code == "68")
    {
        data.sendIntervalMs = 60000;
        data.protocolbuf = readPLCwithWord;
        makeDeviceCode(data.protocolbuf, code);
    }
    else
    {
        // TODO:後で要修正
        // data.protocolbuf = bufD100;
    }

    // デバイスコードを入れる。
    data.deviceCode = code;
}

void MCprotocolManager::makeDeviceCode(vector<char>& buf, string& code)
{
    if (code == "77")
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x4D;
    }
    else if (code == "68")
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x44;
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

vector<vector<string>> MCprotocolManager::convertResponseDataToSendData(char* text, int len, PLCTransactionData& req) {
    string responseData;
    string format;
    vector<vector<string>> sendData;

    for (int i = 0; i < len; ++i)
    {
        if (i > 1)
        {
            responseData += fmt::format("{:02X}", static_cast<unsigned char>(text[i]));

        }
        else
        {
            format += fmt::format("{:02X}", static_cast<unsigned char>(text[i]));
        }
    }

    Logger::getInstance().Info("【シリアルナンバー】" + req.serialNumber + "【フォーマット】"+ format +"【受信データ(2進数)】" + responseData);

    for (int i = 0; i < req.sensorrows.size(); i++)
    {
        string data;
        if (req.deviceCode == "77")
        {
            
            if (i == 0)
            {
                data = responseData.substr(0, 1);
            }
            else
            {
                int startPosition = stoi(req.sensorrows[i][2]) - stoi(req.sensorrows[i - 1][2]);
                data = responseData.substr(startPosition, 1);
            }
        }
        else if (req.deviceCode == "68")
        {
            if (i == 0)
            {
                data = convertDecimalString(swapString(responseData.substr(0, 4)));

            }
            else
            {
                int startPosition = (stoi(req.sensorrows[i][2]) - stoi(req.sensorrows[i - 1][2])) * 4;
                data = convertDecimalString(swapString(responseData.substr(startPosition, 4)));
            }
        }
        // req.sensorrows[i].push_back(req.receiptTime);
        // req.sensorrows[i].push_back(to_string(req.transmissionIntervalMs));
        req.sensorrows[i].push_back(data);
    }
    sendData = req.sensorrows;

    return sendData;
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