#include "CSVIO.hpp"

using namespace std;

vector<map<string, string>> CSVIO::readCSVFileToMapVector(const string& fileName)
{
    vector<map<string, string>> result;
    csv::CSVReader reader(fileName);
    const auto& col_names = reader.get_col_names();

    try {
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

vector<PLCRequestResponseData> CSVIO::makeRequestDataFromMapdata(vector<map<string, string>>& mapdata)
{
    map<string, vector<map<string, string>>> groupedIntervalData;
    map<string, vector<vector<map<string, string>>>> groupedAddressData;

    addASCIIrow(mapdata);
    sortData(mapdata);
    groupedIntervalData = groupMapCataByInterval(mapdata);
    makeDataLumpFromIntervalData(groupedIntervalData);
    groupedAddressData = groupGroupDataByASCII(groupedIntervalData);

    return  makeRequestDataFromMapdata(groupedAddressData);

    // return groupMapDataByASCII(mapdata);
}

void CSVIO::makeDataLumpFromIntervalData(map<string, vector<map<string, string>>> groupedIntervalData)
{
    vector<DataLump> result;

    for (const auto& [intervalStr, dataVec] : groupedIntervalData) {
        DataLump lump;

        lump.sendIntervalMs = atoi(intervalStr.c_str());

        for (const auto& row : dataVec) {
            // sensorReadyStatusにsensorIDをfalseで格納
            auto it = row.find("sensorID");
            if (it != row.end()) {
                map<string, bool> sensorStatus;
                sensorStatus[it->second] = false;
                lump.sensorReadyStatus.push_back(sensorStatus);
            }
        }

        result.push_back(std::move(lump));
    }

    // result確認
    for (const auto& lump : result) {
        cout << "Interval " << lump.sendIntervalMs << "ミリ秒" << endl;
        for (const auto& sensorStatus : lump.sensorReadyStatus) {
            for (const auto& [sensorID, status] : sensorStatus) {
                cout << "Sensor ID: " << sensorID << ", Status: " << status << endl;
            }
        }
    }

    gDataLump = result;
}

void CSVIO::addASCIIrow(vector<map<string, string>>& mapdata)
{
    vector<map<string, string>> result;
    for (auto& row : mapdata) {
        map<string, string> newRow = row;
        newRow["ASCII"] = convertASCIIstring(row["device"]);
        result.push_back(move(newRow));
    }
    mapdata = move(result);
}

vector<PLCRequestResponseData> CSVIO::readCSVFile(const string& fileName)
{
    vector<vector<string>> csvdata;

    try {
        csv::CSVReader reader(fileName);

        const auto headers = reader.get_col_names();

        bool breakFlag = false;
        for (auto& row : reader) {    
            vector<string> rowdata;
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
            csvdata.push_back(move(rowdata));
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
    catch (const exception& e){
        Logger::getInstance().Error("CSVファイルの読込に失敗しました。");
        exit(1);
    }
}

void CSVIO::sortData(vector<map<string, string>>& mapdata)
{
    sort(mapdata.begin(), mapdata.end(),
        [](const auto& a, const auto& b) {
            return stol(a.at("ASCII")) < stol(b.at("ASCII"));
        }
    );
}

void CSVIO::sortData(vector<vector<string>>& csvdata)
{
        sort(csvdata.begin(), csvdata.end(),
        [](auto &a, auto &b) {
            // stol で文字列を long に変換
            return stol(a[2]) < stol(b[2]);
        }
    );
}

string CSVIO::convertASCIIstring(string str)
{
    string initialcode;
    string codenumber;
    string addressscode;
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
            initialcode += to_string(num); // 大文字をASCIIコードに変換
        }
        else if (c >= 97 && c <= 122) // a-z
        {
            int num = (int)c - 32; // 小文字を大文字に変換
            initialcode += to_string(num);
        }
        else if (c >= 48 && c <= 57) // 0-9
        {
            codenumber += to_string(c - 48); // 数字をそのままstringに追加
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

map<string, vector<map<string, string>>> CSVIO::separateMapData(vector<map<string, string>>& mapdata)
{
    map<string, vector<map<string, string>>> separateData;

    for (auto& row : mapdata) {
        string addresscode = row["ASCII"];
        string initialstring = addresscode.erase(addresscode.size() - 4);
        separateData[initialstring].push_back(row);
    }

    return separateData;
}

map<string, vector<map<string, string>>> CSVIO::groupMapCataByInterval(const vector<map<string, string>>& mapdata)
{
    map<string, vector<map<string, string>>> result;


    for (const auto& entry : mapdata) {
        auto it = entry.find("sendIntervalMS");
        if (it != entry.end()) {
            const string& interval = it->second;
            result[interval].push_back(entry);
        }
    }
    vector<map<string, bool>> tempMemory;

    // 結果の表示（確認用）
    for (const auto& [interval, group] : result) {
        cout << "Interval: " << interval << "ms\n";
        for (const auto& row : group) {
            cout << "  { ";
            for (const auto& [key, val] : row) {
                cout << key << ": " << val << ", ";
            }
            cout << "}\n";
        }
    }

    return result;
}

map<string, vector<vector<map<string, string>>>> CSVIO::groupGroupDataByASCII(
    map<string, vector<map<string, string>>>& intervalGroups)
{
    map<string, vector<vector<map<string, string>>>> finalGroups;

    for (auto& [interval, rows] : intervalGroups) {
        // ASCII順にソート
        sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) {
            return getASCIIValue(a) < getASCIIValue(b);
        });

        vector<vector<map<string, string>>> groups;
        vector<map<string, string>> currentGroup;
        int prevASCII = -1;

        for (const auto& row : rows) {
            int ascii = getASCIIValue(row);
            if (!currentGroup.empty() && (ascii - prevASCII > getInterval(row.at("ASCII")))) {
                groups.push_back(currentGroup);
                currentGroup.clear();
            }
            currentGroup.push_back(row);
            prevASCII = ascii;
        }

        if (!currentGroup.empty()) {
            groups.push_back(currentGroup);
        }

        finalGroups[interval] = groups;
    }

    // 確認表示
    for (const auto& [interval, groups] : finalGroups) {
        cout << "Interval: " << interval << "ms\n";
        int groupNo = 1;
        for (const auto& group : groups) {
            cout << "  Group " << groupNo++ << ":\n";
            for (const auto& row : group) {
                cout << "    { ";
                for (const auto& [k, v] : row) {
                    cout << k << ": " << v << ", ";
                }
                cout << "}\n";
            }
        }
    }
    return finalGroups;
}

vector<PLCRequestResponseData> CSVIO::makeRequestDataFromMapdata(
    const map<string, vector<vector<map<string, string>>>>& groupedData)
{
    vector<PLCRequestResponseData> result;
    size_t serialCounter = 1;

    for (const auto& [interval, groups] : groupedData) {
        for (const auto& group : groups) {
            PLCRequestResponseData data;
            if (!group.empty()) {
                data.sendIntervalMs = atoi(group[0].at("sendIntervalMS").c_str());
                data.serialNumber = to_string(serialCounter++);
            }

            for (const auto& row : group) {
                data.mapdata.push_back(row);
            }

            result.push_back(data);
        }
    }

    // resultを確認
    for (const auto& data : result) {
        cout << "Serial Number: " << data.serialNumber << ", Send Interval: " << data.sendIntervalMs << "ms\n";
        for (const auto& row : data.mapdata) {
            cout << "  { ";
            for (const auto& [key, val] : row) {
                cout << key << ": " << val << ", ";
            }
            cout << "}\n";
        }
    }

    return result;
}

// TODO：どこかにワード数、点数の定数を定義する必要がある。
// TODO：後日全てのイニシャルを準備する。今はDとMだけ
int CSVIO::getInterval(string ASCIIstr) {
    string initialstring = ASCIIstr.erase(ASCIIstr.size() - 4);
    if (initialstring == "68") { //D
        return 64; //64ワード
    }
    else if (initialstring == "77") {
        return 256; //256点
    }
    else {
        return 0; // 不明な場合は0を返す
    }
}

int CSVIO::getASCIIValue(const map<string, string>& row) {

    auto it = row.find("ASCII");
    if (it != row.end()) {
        return stoi(it->second);
    }
    return -1; // エラー扱い
}

vector<PLCRequestResponseData> CSVIO::groupMapDataByASCII(
    const vector<map<string, string>>& mapdata)
{
    vector<PLCRequestResponseData> result;

    // コピーしてソート対象にする
    vector<map<string, string>> sorted = mapdata;

    PLCRequestResponseData currentGroup;
    long prevASCII = -999999;
    long preInterval = -999999;
    int serialCounter = 1;

    map<string, string> tempMemory;

    size_t i = 0;
    for (const auto& row : sorted) {
        long currentASCII = stol(row.at("ASCII"));
        long currentInterval = stoi(row.at("sendIntervalMS"));
        if (i == 0)
        {
            preInterval = currentInterval;
        }

        if ((!currentGroup.mapdata.empty() && (currentASCII - prevASCII > SeparateIntarbal))//1つのまとまりを作るデータ間隔を60にする
            || (currentInterval != preInterval)) // 送信間隔が変わった場合 
        {
            // 一つのまとまりとして完了 → 結果に追加
            currentGroup.serialNumber = to_string(serialCounter++);
            currentGroup.sendIntervalMs = row.at("sendIntervalMS").empty() ? 0 : stoi(row.at("sendIntervalMS")) * 1000;
            result.push_back(move(currentGroup));
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
        currentGroup.serialNumber = to_string(serialCounter++);
        currentGroup.sendIntervalMs = tempMemory.at("sendIntervalMS").empty() ? 0 : stoi(tempMemory.at("sendIntervalMS")) * 1000;
        result.push_back(move(currentGroup));
    }

    return result;
}

map<string, vector<vector<string>>> CSVIO::separateCSVData(vector<vector<string>>& csvdata)
{
    map<string, vector<vector<string>>> separateData;

    for (auto& row : csvdata) {
        string addresscode = row[2];
        string initialstring = addresscode.erase(addresscode.size() - 4);
        separateData[initialstring].push_back(row);
    }

    return separateData;
}

vector<PLCRequestResponseData> CSVIO::convertCSVDataToPLCRequestData(const map<string, vector<vector<string>>>& csvdata)
{
    vector<PLCRequestResponseData> gRData;
    int serialNumber = 1;
    for (auto& [prefix, rows] : csvdata) {
        PLCRequestResponseData current;
        int prevValue = 0;
        bool firstRow = true;  

        // 行を順にチェック
        for (auto& row : rows) {
            int value = stoi(row[2]);
            // 1行目はそのまま current に追加
            if (!firstRow && value - prevValue > SeparateIntarbal) {
                // ここまでの current を確定して gRData に追加
                current.serialNumber = to_string(serialNumber++);
                gRData.push_back(move(current));

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
            current.serialNumber = to_string(serialNumber++);
            gRData.push_back(move(current));
        }
    }

    // データ間隔算出
    for (auto& data : gRData) {
        for (size_t i = 0; i < data.sensorrows.size(); ++i) {
            if (i > 0) {
                int prevValue = stoi(data.sensorrows[i - 1][2]);
                int currentValue = stoi(data.sensorrows[i][2]);
                data.dataInterval.push_back(currentValue - prevValue);
                cout << "データ間隔: " << data.dataInterval.back() << endl;
            }
        }
    }

    // 確認出力
    for (auto& data : gRData) {
        cout << "----- serial = " << data.serialNumber << " -----\n";
        for (auto& row : data.sensorrows) {
            for (auto& col : row) cout << col << " ";
            cout << "\n";
        }
    }
    return gRData;
}
