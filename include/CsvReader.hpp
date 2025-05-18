#pragma once
#include <string>
#include <vector>
#include <map>

class CsvReader {
public:
    std::vector<std::map<std::string, std::string>> readWithHeader(const std::string& filename);
    std::vector<std::vector<std::string>> read(const std::string& filename);
};