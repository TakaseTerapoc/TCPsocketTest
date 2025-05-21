#include "CsvReader.hpp"

vector<map<string, string>> CsvReader::readCSVFileToMapVector(const string& fileName)
{
    vector<map<string, string>> result;
    csv::CSVReader reader(fileName);
    const auto& col_names = reader.get_col_names();

    try 
    {
        for (csv::CSVRow& row : reader) {
            map<string, string> rowMap;

            for (const string& col : col_names) {
                rowMap[col] = row[col].get<>();
            }

            result.push_back(move(rowMap));
        }
        if(result.size() == 0)
        {
            Logger::getInstance().Error("CSVファイルのデータがありません。");
            exit(1);
        }
        return result;
    }
    catch (const exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        exit(1);
    }
}