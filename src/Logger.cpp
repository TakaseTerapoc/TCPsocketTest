#include "../include/Logger.hpp"

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::Init()
{
    // ロガーが初期化済みなら何もしない
    if (m_logger != nullptr) return;

    // スレッドプールの初期化　キューサイズ8192　スレッド数1
    spdlog::init_thread_pool(8192, 1);

    // メインロガー用のパターン
    spdlog::set_pattern("[%^%l%$] %v"); 

    vector<spdlog::sink_ptr> sinks; // シンクのベクター
    
    // コンソールシンクとファイルシンクを作成
// #ifdef DEBUG
    auto console_sink = make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.push_back(console_sink);
// #endif
    auto file_sink = make_shared<spdlog::sinks::daily_file_sink_mt>("../logs/SystemLogs/SystemLog", 0, 0);     // ファイルシンク 0時0分にローテーション
    sinks.push_back(file_sink);

    // スレッドプールを使用する非同期メインロガーを作成(async_loggerのコンストラクタをmake_sharedで包んでる)
    m_logger = make_shared<spdlog::async_logger>( 
        "async_logger",                         // ロガー名
        sinks.begin(), sinks.end(),             // 複数のシンクを指定
        spdlog::thread_pool(),                  // スレッドプールを指定
        spdlog::async_overflow_policy::block    // キューが満杯になったらブロックする
    );

    m_logger->set_pattern("[%^%l%$] %v"); // m_loggerのログフォーマット

    // ログを出すたびに出力
    m_logger->flush_on(spdlog::level::debug);

#ifdef DEBUG
    m_logger->set_level(spdlog::level::debug);
#else
    m_logger->set_level(spdlog::level::info);
#endif

    spdlog::set_default_logger(m_logger);

    // センサー用ロガー
    m_sensor_logger = make_shared<spdlog::async_logger>(
        "sensor_logger",                                                             // ロガー名 
        initializer_list<spdlog::sink_ptr>{
            make_shared<spdlog::sinks::daily_file_sink_mt>("../logs/SensorLogs/SensorLog", 0, 0) // センサー用のファイルシンク
        },
        spdlog::thread_pool(),                                                       // スレッドプールを指定
        spdlog::async_overflow_policy::block                                         // キューが満杯になったらブロックする
    );

    // センサー用ロガーはinfo固定
    m_sensor_logger->set_level(spdlog::level::info);

    // ログを吐き出すたびに出力
    m_sensor_logger->flush_on(spdlog::level::debug);


    // センサー用のログフォーマット
    m_sensor_logger->set_pattern("%v"); 

    // spdlog::flush_every(chrono::seconds(1)); 
}

void Logger::Debug(const string& message)
{
    string timestamp = GetCurrentTimestampString();
    m_logger->debug("[{}] {}", timestamp, message);
}

void Logger::Info(const string& message)
{
    string timestamp = GetCurrentTimestampString();
    m_logger->info("[{}] {}", timestamp, message);
}

void Logger::Warn(const string& message)
{
    string timestamp = GetCurrentTimestampString();
    m_logger->warn("[{}] {}", timestamp, message);
}

void Logger::Error(const string& message)
{
    string timestamp = GetCurrentTimestampString();
    m_logger->error("[{}] {}", timestamp, message);
}

void Logger::Sensor(const string& message)
{
    if (m_sensor_logger) {
        string timestamp = GetCurrentTimestampString();
        m_sensor_logger->info("[{}] {}", timestamp, message);
    }
    else {
        m_sensor_logger->error("Sensor logger is not initialized.");
    }
}

void Logger::Flush()
{
    if (m_logger && spdlog::thread_pool()) {
        m_logger->flush();
    }
    if (m_sensor_logger && spdlog::thread_pool()) {
        m_sensor_logger->flush();
    }
}

string Logger::GetCurrentTimestampString() {
    auto now = chrono::system_clock::now();
    auto t_c = chrono::system_clock::to_time_t(now);
    tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t_c);
#else
    localtime_r(&t_c, &tm);
#endif

    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");

    // ミリ秒の計算
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // ミリ秒を "000"〜"999" の3桁ゼロ埋めで追加
    oss << '.' << setfill('0') << setw(3) << ms.count();
    timestamp = oss.str();
    return oss.str();
}