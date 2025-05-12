#pragma once

#include <vector>
#include <map>
#include <string>
#include "../external/csv-parser/csv.hpp"
#include "Logger.hpp"
#include "PLCRequestData.hpp"

const int SeparateIntarbal = 60; // データの分離間隔

/**
* @param CSVファイルの読み書きを行うクラスです。
*/
class CSVIO{
    public:
        // CSVデータを取り出す関数
        static std::vector<PLCRequestData> readCSVFile(const std::string& fileName);

        // センサーデータを昇順にソートする関数
        static void sortData(std::vector<std::vector<std::string>>& csvdata);

        // 文字列からASCIIコードに変換する関数
        // 例) "A100" -> "650100"
        // ２文字目までに文字が含まれない場合はエラーを出力する。
        static std::string convertASCIIstring(std::string str);

        // 取り出したCSVデータをアドレスカテゴリーごとに分ける関数
        static std::map<std::string, std::vector<std::vector<std::string>>> separateCSVData(std::vector<std::vector<std::string>>& csvdata);

        // カテゴリーごとに分けたcsvデータをPLCRequestDataに格納する関数
        static std::vector<PLCRequestData> convertCSVDataToPLCRequestData(const std::map<std::string, std::vector<std::vector<std::string>>>& csvdata);
};