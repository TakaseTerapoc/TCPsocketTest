#pragma once

#include <vector>
#include <map>
#include <string>
#include "../external/csv-parser/csv.hpp"
#include "Logger.hpp"
#include "PLCRequestResponseData.hpp"

const int SeparateIntarbal = 60; // データの分離間隔

/**
* @param CSVファイルの読み書きを行うクラスです。
*/
class CSVIO{
    public:
        // 【新】
        // CSVデータをmap形式で取り出す関数
        static std::vector<std::map<std::string, std::string>> readCSVFileToMapVector(const std::string& fileName);

        // mapデータからPLCRequestResponseDataを作成する関数
        static std::vector<PLCRequestResponseData> makeRequestDataFromMapdata(std::vector<std::map<std::string, std::string>>& mapdata);

        // asciiコードの行をmapに追加
        static void addASCIIrow(std::vector<std::map<std::string, std::string>>& mapdata);

        // map形式のデータをソートする関数
        static void sortData(std::vector<std::map<std::string, std::string>>& mapdata);

        // アドレスの位置を見てmapdataをPLCRequestResponseDataに格納する
        static std::vector<PLCRequestResponseData> groupMapDataByASCII(const std::vector<std::map<std::string, std::string>>& mapdata);

        // 取り出したCSVデータをアドレスカテゴリーごとに分ける関数
        static std::map<std::string, std::vector<std::map<std::string, std::string>>> separateMapData(std::vector<std::map<std::string, std::string>>& mapdata);


        // 【旧】
        // CSVデータを取り出す関数
        static std::vector<PLCRequestResponseData> readCSVFile(const std::string& fileName);

        // センサーデータを昇順にソートする関数
        static void sortData(std::vector<std::vector<std::string>>& csvdata);

        // 文字列からASCIIコードに変換する関数
        // 例) "A100" -> "650100"
        // ２文字目までに文字が含まれない場合はエラーを出力する。
        static std::string convertASCIIstring(std::string str);

        // 取り出したCSVデータをアドレスカテゴリーごとに分ける関数
        static std::map<std::string, std::vector<std::vector<std::string>>> separateCSVData(std::vector<std::vector<std::string>>& csvdata);

        // カテゴリーごとに分けたcsvデータをPLCRequestDataに格納する関数
        static std::vector<PLCRequestResponseData> convertCSVDataToPLCRequestData(const std::map<std::string, std::vector<std::vector<std::string>>>& csvdata);
};