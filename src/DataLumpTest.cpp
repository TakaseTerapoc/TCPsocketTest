#include "DataLumpTest.hpp"

    void DataLumpTest::setAAAA1(const std::map<std::string, std::string>& strVector)
    {
        AAAA1 = strVector;
        Logger::getInstance().Info("1データが入りました。");
        if (AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA2(const std::map<std::string, std::string>& strVector)
    {
        AAAA2 = strVector;
        Logger::getInstance().Info("2データが入りました。");
        if (AAAA1.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA4(const std::map<std::string, std::string>& strVector)
    {
        AAAA4 = strVector;
        Logger::getInstance().Info("4データが入りました。");
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA7(const std::map<std::string, std::string>& strVector)
    {
        AAAA7 = strVector;
        Logger::getInstance().Info("7データが入りました。");
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA10(const std::map<std::string, std::string>& strVector)
    {
        AAAA10 = strVector;
        Logger::getInstance().Info("10データが入りました。");
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA99(const std::map<std::string, std::string>& strVector)
    {
        AAAA99 = strVector;
        Logger::getInstance().Info("99データが入りました。");
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Info("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::allClear()
    {
        AAAA1.clear();
        AAAA2.clear();
        AAAA4.clear();
        AAAA7.clear();
        AAAA10.clear();
        AAAA99.clear();
    }

    std::vector<std::map<std::string,std::string>> DataLumpTest::getMembers() const
    {
        std::vector<std::map<std::string,std::string>> members;
        members.push_back(AAAA1);
        members.push_back(AAAA2);
        members.push_back(AAAA4);
        members.push_back(AAAA7);
        members.push_back(AAAA10);
        members.push_back(AAAA99);
        return members;
    }