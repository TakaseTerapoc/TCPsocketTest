#include "MCprotocolData.hpp"

char buf[] = {
    (char)0x50,(char)0x00,      // サブヘッダ
    (char)0x00,           // ネットワーク番号（アクセス経路）
    (char)0xFF,           // PC番号（アクセス経路）
    (char)0xFF,(char)0x03,      // 要求先ユニットI/O番号(アクセス経路)
    (char)0x00,           // 要求ユニット局番号 (アクセス経路)
    (char)0x00,(char)0x18,      // 要求データ長
    (char)0x20,(char)0x00,      // 監視タイマ
    (char)0x01,(char)0x04,      // コマンド
    (char)0x00,(char)0x00,      // サブコマンド
    (char)0x00,(char)0x00,(char)0x64, // 先頭デバイス番号
    (char)0xA8,           // デバイスコード
    (char)0x09,(char)0x00,      // デバイス点数
};

const char bufD8400[] = {
    (char)0x01,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x0a,(char)0x00,           // 監視タイマ
    (char)0xD0,(char)0x20,(char)0x00,(char)0x00, // 先頭デバイス番号
    (char)0x20,(char)0x44,           // デバイスコード
    (char)0x02                // デバイス点数 
};

const char bufD100[] = {
    (char)0x01,
    (char)0xff,
    (char)0x00,(char)0x00,
    (char)0x64,(char)0x00,(char)0x00,(char)0x00,
    (char)0x20,(char)0x44,
    (char)0x01,
    (char)0x00
};

char bufM10[] = {
    (char)0x00,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ
    (char)0x0a,(char)0x00,(char)0x00,(char)0x00, // 先頭デバイス番号
    (char)0x20,(char)0x4D,           // デバイスコード
    (char)0x02,                // デバイス点数
    (char)0x00, 
};

char bufM00Write[] = {
    (char)0x03,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ
    (char)0x64,(char)0x00,(char)0x00,(char)0x00, // 先頭デバイス番号
    (char)0x20,(char)0x4D,           // デバイスコード
    (char)0x01,                // デバイス点数 
    (char)0x00,
};

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

void MCprotocolData::covertToMCprotocolData(std::vector<PLCRequestData>& gRData)
{
    for (auto& data : gRData)
    {
        std::string code;
        std::string address;
        int firstNumber = 0;
        int lastNumber = 0;
        Logger::getInstance().Info("1");
        // コマンドに応じてMCプロトコルを変換
        if (data.command == "read")
        {
            size_t i = 0;
            std::string code;
            std::string address;
            // for (auto& row : data.csvrows)
            Logger::getInstance().Info("2");
            for (int i = 0; i < (data.csvrows).size(); i++)
            {
                auto& row = data.csvrows[i];
                if(i == 0)
                {
                    makeCommand(row, data, code, address);
                    makeTopDeviceNumber(data.protocolbuf, address);
                    makeDeviceCode(data.protocolbuf, code);
                    firstNumber = std::stoi(row[2]);
                } 
                else if (i == (data.csvrows).size() - 1)
                {
                    lastNumber = std::stoi(row[2]);
                }
            }
            makeDevicePoint(data.protocolbuf, firstNumber, lastNumber);

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

std::string MCprotocolData::substrBack(std::string& str, size_t pos, size_t len) {
    const size_t strLen = str.length();

    return str.substr(strLen - pos, len);
}
void MCprotocolData::makeCommand(std::vector<std::string>& row, PLCRequestData& data, std::string& code, std::string& address)
{
    address = row[2].substr(row[2].size() - 4);
    code = row[2].substr(0, row[2].size() - 4);
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
        // TODO:後で要修正
        // data.protocolbuf = bufD100;
    }
}

void MCprotocolData::makeDeviceCode(std::vector<char>& buf, std::string& code)
{
    if (code == "77" || code == "88")
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x44;
    }
    else if (code == "68")
    {
        buf[8] = (char)0x20;
        buf[9] = (char)0x4D;
    }
}

void MCprotocolData::makeTopDeviceNumber(std::vector<char>& buf, std::string& address)
{
    Logger::getInstance().Info("5");
    Logger::getInstance().Info(address);
    auto bytes = decStrToBytes32(address);
    Logger::getInstance().Info("7");
    for (int i = 0; i < 4; ++i) {
        buf[4 + i] = bytes[i];
    }
}

// 10進文字列を32ビットのリトルエンディアン4バイトに分解する関数      
std::array<uint8_t,4> MCprotocolData::decStrToBytes32(const std::string& decStr) 
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
void MCprotocolData::makeDevicePoint(std::vector<char>& buf, int firstNumber, int lastNumber)
{
    if (lastNumber == 0)
    {
        buf[10] = (char)0x01;
    }
    else
    {
    int deviceCount = lastNumber - firstNumber + 1;
    buf[10] = (char)(deviceCount & 0xFF);
    }
}