#pragma once

#include <string>

using namespace std;

/**
 * @param ファイル読込の文字列リソースを管理するクラスです。
 */

class FileResources {
    public:
        // 各ファイルのパスを記載したファイルのパス
        inline static const string FILEPATHCONFIGFILE = "../ini/filepathconfig.ini";

        inline static const string FILEPATH = "filePath";

        inline static const string CONFIGFILEPATH = "configFilePath";

        inline static const string REQUESTFILEPATH = "requestFilePath"; 
};