#include <vector>
#include <map>
#include <string>

#include "Logger.hpp"

using namespace std;

/**
* @param 読み込んだデータを加工するクラスです。
*/
class DataPreprocessor {
    public:
        // CSVデータをmap形式で取り出す関数
        static vector<map<string, string>> readCSVFileToMapVector(const string& fileName);
};