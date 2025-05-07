#include "CSVIO.hpp"

std::vector<std::vector<std::string>> CSVIO::readCSVFile(const std::string& fileName)
{
    std::vector<std::vector<std::string>> csvdata;

    try {
        csv::CSVReader reader(fileName);

        const auto headers = reader.get_col_names();

        for (auto& row : reader) {
            std::vector<std::string> rowdata;
            rowdata.reserve(headers.size());
            for (int i = 0; i < headers.size(); i++) {
                auto val = row[i].get<>();
                rowdata.push_back(val);
                Logger::getInstance().Info(val);
            }
            csvdata.push_back(std::move(rowdata));
        }
        return csvdata;
    }
    catch (const std::exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        return csvdata;
    }
}