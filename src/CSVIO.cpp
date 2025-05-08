#include "CSVIO.hpp"

std::vector<std::vector<std::string>> CSVIO::readCSVFile(const std::string& fileName)
{
    std::vector<std::vector<std::string>> csvdata;

    try {
        csv::CSVReader reader(fileName);

        const auto headers = reader.get_col_names();

        bool breakFlag = false;
        for (auto& row : reader) {    
            std::vector<std::string> rowdata;
            rowdata.reserve(headers.size());
            for (int i = 0; i < headers.size(); i++) {
                auto val = row[i].get<>();
                if (val.empty()){
                    breakFlag = true;
                    break;
                }
                rowdata.push_back(val);
                if (i == 1) { 
                    // 2列目のデータをASCIIコードに変換する
                    rowdata.push_back(convertASCIIstring(val));
                }
            }
            if (breakFlag){
                breakFlag = false;
                continue;
            }
            csvdata.push_back(std::move(rowdata));
        }
        // 4列目のデータ順に並び替える
        sortData(csvdata);
        return csvdata;
    }
    catch (const std::exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        return csvdata;
    }
}

void CSVIO::sortData(std::vector<std::vector<std::string>>& csvdata)
{
        std::sort(csvdata.begin(), csvdata.end(),
        [](auto &a, auto &b) {
            // std::stol で文字列を long に変換
            return std::stol(a[2]) < std::stol(b[2]);
        }
    );
}

std::string CSVIO::convertASCIIstring(std::string str)
{
    std::string initialcode;
    std::string codenumber;
    std::string addressscode;
    size_t i = 0;
    
    // 最初の英文字をASCIIコードに変換してstringにする。
    for (char c : str) {
        if(i == 2 && initialcode.empty())
        {
            Logger::getInstance().Error("アドレスコードの２文字目までに文字が含まれていません。");
            exit(1);
        }
        if (c >= 65 && c <= 90) // A-Z
        { 
            int num = (int)c;
            initialcode += std::to_string(num); // 大文字をASCIIコードに変換
        }
        else if (c >= 97 && c <= 122) // a-z
        {
            int num = (int)c - 32; // 小文字を大文字に変換
            initialcode += std::to_string(num);
        }
        else if (c >= 48 && c <= 57) // 0-9
        {
            codenumber += std::to_string(c - 48); // 数字をそのままstringに追加
        }
        else
        {
            Logger::getInstance().Error("アドレスコードに不正な文字が含まれています。");
            exit(1);
        }        
        i++;
    }
    // 文字列の長さが4未満の場合、0を追加する
    if (codenumber.length() < 4)
    {
        codenumber.insert(codenumber.begin(), 4 - codenumber.length(), '0');
    }

    addressscode = initialcode + codenumber;
    return addressscode;
}

