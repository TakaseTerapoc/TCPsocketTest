#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>


#include "Logger.hpp"

using namespace std;

/**
* @param タイムスタンプを管理するクラスです。
*/
class TimeStampManager {
    public:

    /**
     * @brief 現在のタイムスタンプを "YYYY-MM-DD HH:MM:SS.mmm" の形式で取得する関数
    */
    inline static string GetCurrentTimestampStringMS() {
        auto now = chrono::system_clock::now();
        auto t_c = chrono::system_clock::to_time_t(now);
        tm tm;

        localtime_r(&t_c, &tm);

        ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");

        // ミリ秒の計算
        auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // ミリ秒を "000"〜"999" の3桁ゼロ埋めで追加
        oss << '.' << setfill('0') << setw(3) << ms.count();
        return oss.str();
    }

    /**
     * @brief 現在のタイムスタンプを "YYYY-MM-DD HH:MM:SS" の形式で取得する関数
     *        ミリ秒は含まれません。
     */
    inline static string GetCurrentTimestampStringSec() {
        auto now = chrono::system_clock::now();
        auto t_c = chrono::system_clock::to_time_t(now);
        tm tm;

        localtime_r(&t_c, &tm);

        ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");

        return oss.str();
    }
};