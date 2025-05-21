#pragma once

#include <vector>
#include <map>
#include <string>
#include "../external/csv-parser/csv.hpp"
#include "Logger.hpp"
#include "PLCRequestResponseData.hpp"
#include "globals.hpp"
#include "DataLump.hpp"

using namespace std;

/**
* @param CSVファイルの読み書きを行うクラスです。
*/
class CSVIO{
    public:
        // 【新】
        // CSVデータをmap形式で取り出す関数
        static vector<map<string, string>> readCSVFileToMapVector(const string& fileName);

        // mapデータからPLCRequestResponseDataを作成する関数
        static vector<PLCRequestResponseData> makeRequestDataFromMapdata(vector<map<string, string>>& mapdata);

        // asciiコードの行をmapに追加
        static void addASCIIrow(vector<map<string, string>>& mapdata);

        // map形式のデータをソートする関数
        static void sortData(vector<map<string, string>>& mapdata);

        // 送信間隔をみてグループ分けする関数
        static map<string, vector<map<string, string>>> groupMapCataByInterval(const vector<map<string, string>>& mapdata);

        // アドレスアスキーをみてグループ分けする関数
        static map<string, vector<vector<map<string, string>>>> groupGroupDataByASCII(map<string, vector<map<string, string>>>& intervalGrouped);

        // グループ分けしたデータをPLCRequestResponseDataに格納する関数
        static vector<PLCRequestResponseData> makeRequestDataFromMapdata(const map<string, vector<vector<map<string, string>>>>& groupedData);

        // DataLumpを作成する関数
        static void makeDataLumpFromIntervalData(map<string, vector<map<string, string>>> groupedIntervalData);

        // デバイスのイニシャル文字コードを見て、一度にデータが取れる範囲を決定する。
        static int getInterval(string ASCIIstr);

        static int getASCIIValue(const map<string, string>& row);

        // アドレスの位置を見てmapdataをPLCRequestResponseDataに格納する
        static vector<PLCRequestResponseData> groupMapDataByASCII(const vector<map<string, string>>& mapdata);

        // 取り出したCSVデータをアドレスカテゴリーごとに分ける関数
        static map<string, vector<map<string, string>>> separateMapData(vector<map<string, string>>& mapdata);

        // データの分離間隔
        // TODO:本当にこの場所でいいのか。。。要検討
        static const int SeparateIntarbal = 60; 

        // 【旧】
        // CSVデータを取り出す関数
        static vector<PLCRequestResponseData> readCSVFile(const string& fileName);

        // センサーデータを昇順にソートする関数
        static void sortData(vector<vector<string>>& csvdata);

        // 文字列からASCIIコードに変換する関数
        // 例) "A100" -> "650100"
        // ２文字目までに文字が含まれない場合はエラーを出力する。
        static string convertASCIIstring(string str);

        // 取り出したCSVデータをアドレスカテゴリーごとに分ける関数
        static map<string, vector<vector<string>>> separateCSVData(vector<vector<string>>& csvdata);

        // カテゴリーごとに分けたcsvデータをPLCRequestDataに格納する関数
        static vector<PLCRequestResponseData> convertCSVDataToPLCRequestData(const map<string, vector<vector<string>>>& csvdata);
};