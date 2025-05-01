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
    void Error(const std::string& message);

    // センサー情報用のロガー
    void Sensor(const std::string& message);

    // ログをフラッシュする
    void Flush();

private:
    Logger() = default;             // コンストラクタ

    // デストラクタ
    ~Logger() = default;
    
    // コピー禁止
    Logger(const Logger&) = delete;             // コピーコンストラクタ
    Logger& operator=(const Logger&) = delete;  // コピー代入演算子

    std::shared_ptr<spdlog::logger> m_logger;       // メインロガー
    std::shared_ptr<spdlog::logger> m_sensor_logger;// センサー用ロガー
};
