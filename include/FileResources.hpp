#pragma once

#include <string>

using namespace std;

/**
 * @param ファイル読込の文字列リソースを管理するクラスです。
 */

class FileResources {
    public:
        // 設定ファイルのパス
        static const string PLC_CONFIG_FILE = "../ini/config.ini";

        // リクエストファイルのパス
        static const string REQUEST_FILE = "../request/testdataDemo5.csv";
};