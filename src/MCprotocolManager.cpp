#include "MCprotocolManager.hpp"

std::vector<char> readPLCwithBit = 
{
    (char)0x00,                                     // サブヘッダ 0番目
    (char)0xff,                                     // PC番号（アクセス経路）1番目
    (char)0x00,(char)0x00,                          // 監視タイマ 2番目3番目
    (char)0x0a,(char)0x00,(char)0x00,(char)0x00,    // 先頭デバイス番号 4番目5番目6番目7番目
    (char)0x00,(char)0x00,                          // デバイスコード 8番目9番目
    (char)0x00,                                     // デバイス点数 10番目
    (char)0x00,                                     // 終点 11番目
};

std::vector<char> readPLCwithWord = 
{
    (char)0x01,                                     // サブヘッダ 0番目
    (char)0xff,                                     // PC番号（アクセス経路）1番目
    (char)0x00,(char)0x00,                          // 監視タイマ 2番目3番目
    (char)0x0a,(char)0x00,(char)0x00,(char)0x00,    // 先頭デバイス番号 4番目5番目6番目7番目
    (char)0x00,(char)0x00,                          // デバイスコード 8番目9番目
    (char)0x00,                                     // デバイス点数 10番目
    (char)0x00,                                     // 終点 11番目
};


std::vector<char> startPLC = 
{
    (char)0x13,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ
    (char)0x00,
};

std::vector<char> stopPLC = 
{
    (char)0x14,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ      
    (char)0x00,
};


std::vector<char> getPLCName = 
{
    (char)0x15,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,     // 監視タイマ
    (char)0x00,
};

std::vector<char> test = 
{
    (char)0x16,                                     // サブヘッダ
    (char)0xFF,                                     // PC番号（アクセス経路）
    (char)0x00,(char)0x00,                          // 監視タイマ
    (char)0x03,(char)0x01,(char)0x02,(char)0x03     // テストデータ
};

void MCprotocolManager::covertToMCprotocolData(std::vector<PLCRequestResponseData>& gRData)
{
    for (auto& data : gRData)
    {
        std::string code;
        std::string address;
        int firstNumber = 0;
        int lastNumber = 0;

        // コマンドに応じてMCプロトコルを変換
        if (data.command == "read")
        {
            size_t i = 0;
            std::string code;
            std::string address;
            for (int i = 0; i < (data.sensorrows).size(); i++)
            {
                data.deviceCount++;
                auto& row = data.sensorrows[i];
                if(i == 0)
                {
                    makeCommand(row, data, code, address);
                    makeTopDeviceNumber(data.protocolbuf, address);
                    makeDeviceCode(data.protocolbuf, code);
                    firstNumber = std::stoi(row[2]);
                } 
                else if (i == (data.sensorrows).size() - 1)
                {
                    lastNumber = std::stoi(row[2]);
                }
            }
            makeDevicePoint(data, firstNumber, lastNumber);

            // 確認用
            std::cout << "【シリアルナンバー】" << data.serialNumber << std::endl;
            for (std::size_t i = 0; i < data.protocolbuf.size(); ++i) {
                uint8_t byte = static_cast<uint8_t>(data.protocolbuf[i]);
                std::printf("readPLCwithWord[%2zu] = 0x%02X\n", i, byte);
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

std::string MCprotocolManager::substrBack(std::string& str, size_t pos, size_t len) {
    const size_t strLen = str.length();

    return str.substr(strLen - pos, len);
}
void MCprotocolManager::makeCommand(std::vector<std::string>& row, PLCRequestResponseData& data, std::string& code, std::string& address)
{
    address = row[2].substr(row[2].size() - 4);
    code = row[2].substr(0, row[2].size() - 4);
    if (code == "77" || code == "88")
    {
        data.transmissionIntervalMs = 1000;
        data.protocolbuf = readPLCwithBit;
        makeDeviceCode(data.protocolbuf, code);
    }
    else if (code == "68")
    {
        data.transmissionIntervalMs = 60000;
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

void MCprotocolManager::makeDeviceCode(std::vector<char>& buf, std::string& code)
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

void MCprotocolManager::makeTopDeviceNumber(std::vector<char>& buf, std::string& address)
{
    auto bytes = decStrToBytes32(address);
    for (int i = 0; i < 4; ++i) {
        buf[4 + i] = bytes[i];
    }
}

// 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数      
std::array<uint8_t,4> MCprotocolManager::decStrToBytes32(const std::string& decStr) 
{
    unsigned long v = std::stoul(decStr, nullptr, 10);
    return {
        static_cast<uint8_t>( v        & 0xFF),
        static_cast<uint8_t>((v >>  8) & 0xFF),
        static_cast<uint8_t>((v >> 16) & 0xFF),
        static_cast<uint8_t>((v >> 24) & 0xFF)
    };
}

// デバイス点数を作成する関数
void MCprotocolManager::makeDevicePoint(PLCRequestResponseData& data, int firstNumber, int lastNumber)
{
    if ( data.deviceCount == 1)
    {
        data.protocolbuf[10] = (char)0x01;
    }
    else
    {
        data.protocolbuf[10] = (char)((lastNumber - firstNumber + 1) & 0xFF);
    }
}

std::string MCprotocolManager::convertSendData(char* text, int len, PLCRequestResponseData& req) {
    std::string responseData;

    for (int i = 2; i < len; ++i)
    {
        responseData += toBinaryString(text[i]);
    }

    std::cout << "受信データ: " << responseData << std::endl;

    for (int i = 0; i < req.sensorrows.size(); i++)
    {
        req.sensorrows[i].push_back();
    }

    for (int i = 0; i < ; ++i)
    {
        uint8_t byte = static_cast<uint8_t>(text[i]);
        std::printf("recv[%2d] = 0x%02X\n", i, byte);
    }

    return responseData;
}

std::string MCprotocolManager::toBinaryString(uint8_t byte) {
    return std::bitset<8>(byte).to_string();
}

std::string MCprotocolManager::makeSendData(uint8_t byte)
{
    std::string binaryString = std::bitset<8>(byte).to_string();
    std::string transmitData;

    for (char bit : binaryString) {
        if (bit == '1') {
            transmitData += "1";
        } else {
            transmitData += "0";
        }
    }

    return transmitData;
}