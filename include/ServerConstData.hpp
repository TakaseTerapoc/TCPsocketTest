#pragma once

#include "Logger.hpp"

using namespace std;

/**
 * @brief サーバーに送る定数データのクラスです。
*/
class ServerConstData {
    public:
        enum DataType {
            DATA_TYPE_LITERAL = 0x00,   // 文字データ
            DATA_TYPE_BINARY = 0x01,    // バイナリデータ
            DATA_TYPE_CONFIRM = 0x02,   // 接続確認用データ
        };

        // データのサイズ
        static const int IpHeaderSize = 20;
        static const int UDPHeaderSize = 8;
        static const int formatHeaderSize = 7;

    private:

};