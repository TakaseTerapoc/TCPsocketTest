#include "CSVIO.hpp"

std::vector<PLCRequestData> CSVIO::readCSVFile(const std::string& fileName)
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
        if(csvdata.size() == 0)
        {
            Logger::getInstance().Error("CSVファイルのデータがありません。");
            exit(1);
        }
        // 4列目のデータ順に並び替える
        sortData(csvdata);
        return convertCSVDataToPLCRequestData(separateCSVData(csvdata));
    }
    catch (const std::exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        exit(1);
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

std::map<std::string, std::vector<std::vector<std::string>>> CSVIO::separateCSVData(std::vector<std::vector<std::string>>& csvdata)
{
    std::map<std::string, std::vector<std::vector<std::string>>> separateData;

    for (auto& row : csvdata) {
        std::string addresscode = row[2];
        std::string initialstring = addresscode.erase(addresscode.size() - 4);
        separateData[initialstring].push_back(row);
    }

    return separateData;
}

std::vector<PLCRequestData> CSVIO::convertCSVDataToPLCRequestData(const std::map<std::string, std::vector<std::vector<std::string>>>& csvdata)
{
    std::vector<PLCRequestData> gRData;
    int serialNumber = 1;
    for (auto& [prefix, rows] : csvdata) {
        PLCRequestData current;
        int prevValue = 0;
        bool firstRow = true;  

        // 行を順にチェック
        for (auto& row : rows) {
            int value = std::stoi(row[2]);
            // 1行目はそのまま current に追加
            if (!firstRow && value - prevValue > SeparateIntarbal) {
                // ここまでの current を確定して gRData に追加
                current.serialNumber = std::to_string(serialNumber++);
                gRData.push_back(std::move(current));

                // 新しいグループ用にリセット
                current = PLCRequestData{};
                firstRow = true;
            }

            // current にこの行を追加
            current.csvrows.push_back(row);
            prevValue = value;
            firstRow = false;
        }

        // 最後に残った current を追加
        if (!current.csvrows.empty()) {
            current.serialNumber = std::to_string(serialNumber++);
            gRData.push_back(std::move(current));
        }
    }

    // データ間隔算出
    for (auto& data : gRData) {
        for (size_t i = 0; i < data.csvrows.size(); ++i) {
            if (i > 0) {
                int prevValue = std::stoi(data.csvrows[i - 1][2]);
                int currentValue = std::stoi(data.csvrows[i][2]);
                data.dataInterval.push_back(currentValue - prevValue);
                std::cout << "データ間隔: " << data.dataInterval.back() << std::endl;
            }
        }
    }

    // 確認出力
    for (auto& data : gRData) {
        std::cout << "----- serial = " << data.serialNumber << " -----\n";
        for (auto& row : data.csvrows) {
            for (auto& col : row) std::cout << col << " ";
            std::cout << "\n";
        }
    }
    return gRData;
}
