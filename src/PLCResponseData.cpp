# include "PLCResponseData.hpp"

PLCResponseData::PLCResponseData()
{
}

void PLCResponseData::setReceiptTime()
{
    using namespace std::chrono;

    // 現在時刻を取得
    auto now = system_clock::now();
    auto ms  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // 秒単位の time_t に変換し tm を得る
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm    = *std::localtime(&t);

    // ostringstream にフォーマットを書き込む
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << '.'
        << std::setw(3) << std::setfill('0') << ms.count();

    // std::string に変換
    receiptTime = oss.str();

    // 確認出力
    std::cout << "取得時刻: " << receiptTime << "\n";
}

// TODO:今後レスポンスデータを加工する必要がある。
void PLCResponseData::setResponseData(char* data)
{
    responseData = data;
}