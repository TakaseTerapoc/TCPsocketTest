#include <vector>
#include <map>
#include <string>

#include "../external/csv-parser/csv.hpp"
#include "Logger.hpp"

using namespace std;

/**
* @param CSVファイルの読み込みを行うクラスです。
*/
class CsvReader {
    public:
        // CSVデータをmap形式で取り出す関数
        static vector<map<string, string>> readCSVFileToMapVector(const string& fileName);
};