#pragma once

#include <vector>
#include <map>
#include <string>
#include "../external/csv-parser/csv.hpp"
#include "Logger.hpp"

/**
* @param CSVファイルの読み書きを行うクラスです。
*/
class CSVIO{
    public:
        // カラムが2列のCSVデータを取り出す関数
        static std::vector<std::vector<std::string>> readCSVFile(const std::string& fileName);

        // センサーデータを分割する関数
        static void sortData(std::vector<std::vector<std::string>>& csvdata);

        // 文字列からASCIIコードに変換する関数
        // 例) "A100" -> "650100"
        // ２文字目までに文字が含まれない場合はエラーを出力する。
        static std::string convertASCIIstring(std::string str);
};