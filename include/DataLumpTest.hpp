#pragma once

#include <vector>
#include <string>
#include "Logger.hpp"
#include "DataLumpBase.hpp"

class DataLumpTest : public DataLumpBase {
public:
    void setAAAA1(std::vector<std::string>& strVector);

    void setAAAA2(std::vector<std::string>& strVector);

    void setAAAA4(std::vector<std::string>& strVector);

    void setAAAA7(std::vector<std::string>& strVector);

    void setAAAA10(std::vector<std::string>& strVector);

    void setAAAA99(std::vector<std::string>& strVector);

    std::vector<std::string> getMembers() const {
         return {
            "AAAA1: " + std::to_string(AAAA1.size()),
            "AAAA2: " + std::to_string(AAAA2.size()),
            "AAAA4: " + std::to_string(AAAA4.size()),
            "AAAA7: " + std::to_string(AAAA7.size()),
            "AAAA10: " + std::to_string(AAAA10.size()),
            "AAAA99: " + std::to_string(AAAA99.size())
         }
    }

private:
    std::vector<std::string> AAAA1;

    std::vector<std::string> AAAA2;

    std::vector<std::string> AAAA4;

    std::vector<std::string> AAAA7;

    std::vector<std::string> AAAA10;

    std::vector<std::string> AAAA99;
};