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

std::vector<char> startPLC = 
{
    (char)0x13,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ
    (char)0x00      
};

std::vector<char> stopPLC = 
{
    (char)0x14,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ      
    (char)0x00
};


std::vector<char> getPLCName = 
{
    (char)0x15,                // サブヘッダ
    (char)0xff,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ
    (char)0x00,              
};

std::vector<char> test = 
{
    (char)0x16,                // サブヘッダ
    (char)0xFF,                // PC番号（アクセス経路）
    (char)0x00,(char)0x00,           // 監視タイマ
    (char)0x03,(char)0x01,(char)0x02,(char)0x03  // テストデータ
};