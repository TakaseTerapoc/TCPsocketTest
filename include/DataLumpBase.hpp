#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

class DataLumpBase {
public:
    virtual ~DataLumpBase() = default;

    bool isLumpFull() const { return lumpFull; }
    void setLumpFull(bool val) { lumpFull = val; }
    virtual std::vector<std::map<std::string,std::string>> getMembers() const = 0;
    virtual void allClear() = 0;

protected:
    bool lumpFull = false;
};