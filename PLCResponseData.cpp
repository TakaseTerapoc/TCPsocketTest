# include "PLCResponseData.hpp"

PLCResponseData::PLCResponseData()
{
}

void PLCResponseData::setReceiptTime()
{
    // 現在時刻を取得
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    
    // 現在時刻から年月日を取得
    boost::gregorian::date d = now.date();
    std::string yyyymmdd = boost::gregorian::to_iso_string(d);

    // 現在時刻から時分秒ミリ秒を取得
    boost::posix_time::time_duration tod = now.time_of_day();
    std::string hhmmssff = to_iso_string(tod);

    // 上で作成した文字列を結合し、receiptTimeに格納
    receiptTime = yyyymmdd + hhmmssff;

    std::cout << receiptTime << std::endl;
}

// TODO:今後レスポンスデータを加工する必要がある。
void PLCResponseData::setResponseData(char* data)
{
    responseData = data;
}