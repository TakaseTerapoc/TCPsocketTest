#pragma once

#include <vector>
#include <string>
#include "Logger.hpp"
#include "DataLumpBase.hpp"

class DataLumpTest : DataLumpBase {
public:
    void setAAAA1(std::vector<std::string>& strVector);

    void setAAAA2(std::vector<std::string>& strVector);

    void setAAAA4(std::vector<std::string>& strVector);

    void setAAAA7(std::vector<std::string>& strVector);

    void setAAAA10(std::vector<std::string>& strVector);

    void setAAAA99(std::vector<std::string>& strVector);

private:
    std::vector<std::string> AAAA1;

    std::vector<std::string> AAAA2;

    std::vector<std::string> AAAA4;

    std::vector<std::string> AAAA7;

    std::vector<std::string> AAAA10;

    std::vector<std::string> AAAA99;
};