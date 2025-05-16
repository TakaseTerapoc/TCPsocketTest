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
    // コンストラクタ
    DataLumpTest() = default;
    
    DataLumpTest(const DataLumpTest& copysource) {
        this->AAAA1 = copysource.AAAA1;
        this->AAAA2 = copysource.AAAA2;
        this->AAAA4 = copysource.AAAA4;
        this->AAAA7 = copysource.AAAA7;
        this->AAAA10 = copysource.AAAA10;
        this->AAAA99 = copysource.AAAA99;
        this->lumpFull = copysource.lumpFull;
    }

    void setAAAA1(const std::map<std::string, std::string>& strVector);

    void setAAAA2(const std::map<std::string, std::string>& strVector);

    void setAAAA4(const std::map<std::string, std::string>& strVector);

    void setAAAA7(const std::map<std::string, std::string>&strVector);

    void setAAAA10(const std::map<std::string, std::string>& strVector);

    void setAAAA99(const std::map<std::string, std::string>& strVector);

    std::vector<std::map<std::string,std::string>> getMembers() const override;

    void allClear() override;

private:
    std::map<std::string,std::string> AAAA1;

    std::map<std::string,std::string> AAAA2;

    std::map<std::string,std::string> AAAA4;

    std::map<std::string,std::string> AAAA7;

    std::map<std::string,std::string> AAAA10;

    std::map<std::string,std::string> AAAA99;
};