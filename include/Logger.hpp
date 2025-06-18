#pragma once

#include "../external/spdlog/include/spdlog/spdlog.h"
#include "../external/spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "../external/spdlog/include/spdlog/sinks/daily_file_sink.h"
#include "../external/spdlog/include/spdlog/async.h"
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;



/**
 * @brief ロガークラスです。
*/
class Logger
{
public:
    // シングルトンインスタンス取得
    static Logger& getInstance();

    // ロガーの初期化
    void Init();

    // デバッグ用のロガー
    void Debug(const std::string& message);

    // Info用のロガー
    void Info(const std::string& message);

    // 警告用のロガー
    void Warn(const std::string& message);

    // エラー用のロガー
    void Error2(const std::string& message, const char* file, int line, const char* func);
    void Error(const std::string& message);

    // センサー情報用のロガー
    void Sensor(const std::string& message);

    // ログをフラッシュする
    void Flush();

private:
    // コンストラクタとデストラクタ
    Logger() = default;
    ~Logger() = default;
    
    // コピー禁止
    Logger(const Logger&) = delete;             // コピーコンストラクタ
    Logger& operator=(const Logger&) = delete;  // コピー代入演算子

    std::shared_ptr<spdlog::logger> m_logger;       // メインロガー
    std::shared_ptr<spdlog::logger> m_sensor_logger;// センサー用ロガー
};

// // Errorログにファイル名・行番号・関数名を自動で含めるマクロ
// #define Error(msg) \
//     Error(msg, __FILE__, __LINE__, __func__)
