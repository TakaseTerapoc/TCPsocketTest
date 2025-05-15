#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include "Logger.hpp"
#include "DataLumpBase.hpp"

class DataLumpTest : public DataLumpBase {
public:
    void setAAAA1(const std::map<std::string, std::string>& strVector);

    void setAAAA2(const std::map<std::string, std::string>& strVector);

    void setAAAA4(const std::map<std::string, std::string>& strVector);

    void setAAAA7(const std::map<std::string, std::string>&strVector);

    void setAAAA10(const std::map<std::string, std::string>& strVector);

    void setAAAA99(const std::map<std::string, std::string>& strVector);

    // std::vector<std::string> getMembers();

private:
    std::map<std::string,std::string> AAAA1;

    std::map<std::string,std::string> AAAA2;

    std::map<std::string,std::string> AAAA4;

    std::map<std::string,std::string> AAAA7;

    std::map<std::string,std::string> AAAA10;

    std::map<std::string,std::string> AAAA99;

    void allClear();
};