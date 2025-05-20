#include "CSVIO.hpp"

std::vector<std::map<std::string, std::string>> CSVIO::readCSVFileToMapVector(const std::string& fileName)
{
    std::vector<std::map<std::string, std::string>> result;
    csv::CSVReader reader(fileName);
    const auto& col_names = reader.get_col_names();

    try {
        for (csv::CSVRow& row : reader) {
            std::map<std::string, std::string> rowMap;

            for (const std::string& col : col_names) {
                rowMap[col] = row[col].get<>();
            }

            result.push_back(std::move(rowMap));
        }
        if(result.size() == 0)
        {
            Logger::getInstance().Error("CSVファイルのデータがありません。");
            exit(1);
        }
        return result;
    }
    catch (const std::exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        exit(1);
    }
}

std::vector<PLCRequestResponseData> CSVIO::makeRequestDataFromMapdata(std::vector<std::map<std::string, std::string>>& mapdata)
{
    addASCIIrow(mapdata);
    sortData(mapdata);
    return groupMapDataByASCII(mapdata);
}

void CSVIO::addASCIIrow(std::vector<std::map<std::string, std::string>>& mapdata)
{
    std::vector<std::map<std::string, std::string>> result;
    for (auto& row : mapdata) {
        std::map<std::string, std::string> newRow = row;
        newRow["ASCII"] = convertASCIIstring(row["device"]);
        result.push_back(std::move(newRow));
    }
    mapdata = std::move(result);
}

std::vector<PLCRequestResponseData> CSVIO::readCSVFile(const std::string& fileName)
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

void CSVIO::sortData(std::vector<std::map<std::string, std::string>>& mapdata)
{
    std::sort(mapdata.begin(), mapdata.end(),
        [](const auto& a, const auto& b) {
            return std::stol(a.at("ASCII")) < std::stol(b.at("ASCII"));
        }
    );
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

std::map<std::string, std::vector<std::map<std::string, std::string>>> CSVIO::separateMapData(std::vector<std::map<std::string, std::string>>& mapdata)
{
    std::map<std::string, std::vector<std::map<std::string, std::string>>> separateData;

    for (auto& row : mapdata) {
        std::string addresscode = row["ASCII"];
        std::string initialstring = addresscode.erase(addresscode.size() - 4);
        separateData[initialstring].push_back(row);
    }

    return separateData;
}

std::vector<PLCRequestResponseData> CSVIO::groupMapDataByASCII(
    const std::vector<std::map<std::string, std::string>>& mapdata)
{
    std::vector<PLCRequestResponseData> result;

    // コピーしてソート対象にする
    std::vector<std::map<std::string, std::string>> sorted = mapdata;

    PLCRequestResponseData currentGroup;
    long prevASCII = -999999;
    long preInterval = -999999;
    int serialCounter = 1;

    std::map<std::string, std::string> tempMemory;

    size_t i = 0;
    for (const auto& row : sorted) {
        long currentASCII = std::stol(row.at("ASCII"));
        long currentInterval = std::stoi(row.at("sendIntervalMS"));
        if (i == 0)
        {
            preInterval = currentInterval;
        }

        if ((!currentGroup.mapdata.empty() && (currentASCII - prevASCII > SeparateIntarbal))//1つのまとまりを作るデータ間隔を60にする
            || (currentInterval != preInterval)) // 送信間隔が変わった場合 
        {
            // 一つのまとまりとして完了 → 結果に追加
            currentGroup.serialNumber = std::to_string(serialCounter++);
            currentGroup.sendIntervalMs = row.at("sendIntervalMS").empty() ? 0 : std::stoi(row.at("sendIntervalMS")) * 1000;
            result.push_back(std::move(currentGroup));
            currentGroup = PLCRequestResponseData(); // 新しいグループ
        }

        currentGroup.mapdata.push_back(row);
        prevASCII = currentASCII;
        preInterval = currentInterval;
        if (i == sorted.size() - 1)
        {
            tempMemory = row;
        }
        i++;
    }

    // 最後のグループを追加
    if (!currentGroup.mapdata.empty()) {
        currentGroup.serialNumber = std::to_string(serialCounter++);
        currentGroup.sendIntervalMs = tempMemory.at("sendIntervalMS").empty() ? 0 : std::stoi(tempMemory.at("sendIntervalMS")) * 1000;
        result.push_back(std::move(currentGroup));
    }

    return result;
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

std::vector<PLCRequestResponseData> CSVIO::convertCSVDataToPLCRequestData(const std::map<std::string, std::vector<std::vector<std::string>>>& csvdata)
{
    std::vector<PLCRequestResponseData> gRData;
    int serialNumber = 1;
    for (auto& [prefix, rows] : csvdata) {
        PLCRequestResponseData current;
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
                current = PLCRequestResponseData{};
                firstRow = true;
            }

            // current にこの行を追加
            current.sensorrows.push_back(row);
            prevValue = value;
            firstRow = false;
        }

        // 最後に残った current を追加
        if (!current.sensorrows.empty()) {
            current.serialNumber = std::to_string(serialNumber++);
            gRData.push_back(std::move(current));
        }
    }

    // データ間隔算出
    for (auto& data : gRData) {
        for (size_t i = 0; i < data.sensorrows.size(); ++i) {
            if (i > 0) {
                int prevValue = std::stoi(data.sensorrows[i - 1][2]);
                int currentValue = std::stoi(data.sensorrows[i][2]);
                data.dataInterval.push_back(currentValue - prevValue);
                std::cout << "データ間隔: " << data.dataInterval.back() << std::endl;
            }
        }
    }

    // 確認出力
    for (auto& data : gRData) {
        std::cout << "----- serial = " << data.serialNumber << " -----\n";
        for (auto& row : data.sensorrows) {
            for (auto& col : row) std::cout << col << " ";
            std::cout << "\n";
        }
    }
    return gRData;
}
