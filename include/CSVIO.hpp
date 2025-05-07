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
        // カラムが2列のCSVデータを取り出す。
        static std::vector<std::vector<std::string>> readCSVFile(const std::string& fileName);

};