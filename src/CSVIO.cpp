#include "CSVIO.hpp"

// ★
vector<PLCTransactionData> CSVIO::makeRequestDataFromMapdata(vector<map<string, string>>& mapdata)
{
    map<string, vector<map<string, string>>> groupedIntervalData;
    map<string, vector<vector<map<string, string>>>> groupedAddressData;

    addASCIIrow(mapdata);
    sortData(mapdata);
    groupedIntervalData = groupMapDataByInterval(mapdata);

    // DataLumpを作成する
    makeDataLumpFromIntervalData(groupedIntervalData);


    groupedAddressData = groupGroupDataByASCII(groupedIntervalData);

    return  makeRequestDataFromMapdata(groupedAddressData);

    // return groupMapDataByASCII(mapdata);
}

// ★
void CSVIO::makeDataLumpFromIntervalData(map<string, vector<map<string, string>>> groupedIntervalData)
{
    vector<DataLump> result;

    for (const auto& [intervalStr, dataVec] : groupedIntervalData) {
        DataLump lump;

        lump.sendIntervalMs = atoi(intervalStr.c_str()) * 1000; // ミリ秒に変換

        for (const auto& row : dataVec) {
            // sensorReadyStatusにsensorIDをfalseで格納
            auto it = row.find("sensorID");
            if (it != row.end()) {
                map<string, bool> sensorStatus;
                sensorStatus[it->second] = false;
                lump.sensorReadyStatus.push_back(sensorStatus);
            }
        }

        result.push_back(move(lump));
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

// ★
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

// ★
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

// ★
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

// ★
map<string, vector<map<string, string>>> CSVIO::groupMapDataByInterval(const vector<map<string, string>>& mapdata)
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

// ★
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
        int prevASCII = 0;
        int maxASCII = 0;
        bool initflag = false;
        int initASCII = 0;

        size_t i = 0;

        for (const auto& row : rows) {
            if (i == 0) {
                prevASCII = getASCIIValue(row);
                initASCII = prevASCII;
                initflag = true;
            }
            
            int ascii = getASCIIValue(row);
            
            if (!currentGroup.empty() && (ascii - initASCII > getInterval(row.at("ASCII")))) {
                groups.push_back(currentGroup);
                currentGroup.clear();
                initASCII = ascii;
            }
            currentGroup.push_back(row);
            prevASCII = ascii;
            i++;
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

// ★
vector<PLCTransactionData> CSVIO::makeRequestDataFromMapdata(
    const map<string, vector<vector<map<string, string>>>>& groupedData)
{
    vector<PLCTransactionData> result;
    size_t serialCounter = 1;

    for (const auto& [interval, groups] : groupedData) {
        for (const auto& group : groups) {
            PLCTransactionData data;
            if (!group.empty()) {
                data.sendIntervalMs = atoi(group[0].at("sendIntervalMS").c_str()) * 1000;
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

// ★
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

// ★
int CSVIO::getASCIIValue(const map<string, string>& row) {

    auto it = row.find("ASCII");
    if (it != row.end()) {
        return stoi(it->second);
    }
    return -1; // エラー扱い
}