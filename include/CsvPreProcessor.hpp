#pragma once
#include <vector>
#include <map>
#include <string>

class CsvPreprocessor {
public:
    void addAsciiColumn(std::vector<std::map<std::string, std::string>>& rows);
    void sortByAscii(std::vector<std::map<std::string, std::string>>& rows);
};