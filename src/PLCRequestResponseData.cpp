#include "PLCRequestResponseData.hpp"

// std::string READ = "read";
// std::string WRITE = "write";

// void PLCRequestResponseData::convertMCprotocol()
// {
//     std::string commandstr = this->command;
//     std::string dataAddressstr = this->dataAddress;
//     if (commandstr == "getname")
//     {
//         protocolbuf[0] = static_cast<char>(0x15);
//         protocolbuf[0] = static_cast<char>(0x15);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//     }
//     else if (commandstr == "start")
//     {
//         protocolbuf.push_back(0x13);
//         protocolbuf.push_back(0xff);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//     }
//     else if (commandstr == "stop")
//     {
//         protocolbuf.push_back(0x13);
//         protocolbuf.push_back(0xff);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//         protocolbuf.push_back(0x00);
//     }

//     if (commandstr == "read")
//     {
//         char fs = firstChar(dataAddressstr);
//         if (fs == 'D' || fs == 'R' || fs == 'T' || fs == 'C')
//         {
//             protocolbuf.push_back(0x01);
//             protocolbuf.push_back(0xff);
//             protocolbuf.push_back(0x00);
//             protocolbuf.push_back(0x00);
//         }

//     }
// }

// char PLCRequestResponseData::firstChar(const std::string& s) {
//     if (s.empty()) {
//         std::cout << "文字列が空です" << std::endl;
//         exit(1);
//     }
//     return s[0];
// }

// void PLCRequestResponseData::setReceiptTime()
// {
//     using namespace std::chrono;

//     // 現在時刻を取得
//     auto now = system_clock::now();
//     auto ms  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

//     // 秒単位の time_t に変換し tm を得る
//     std::time_t t = system_clock::to_time_t(now);
//     std::tm tm    = *std::localtime(&t);

//     // ostringstream にフォーマットを書き込む
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
//         << '.'
//         << std::setw(3) << std::setfill('0') << ms.count();

//     // std::string に変換
//     receiptTime = oss.str();

//     // 確認出力
//     std::cout << "取得時刻: " << receiptTime << "\n";
// }