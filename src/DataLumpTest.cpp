#include "DataLumpTest.hpp"

    void DataLumpTest::setAAAA1(std::vector<std::string>& strVector)
    {
        AAAA1 = strVector;
        if (AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA2(std::vector<std::string>& strVector)
    {
        AAAA2 = strVector;
        if (AAAA1.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA4(std::vector<std::string>& strVector)
    {
        AAAA4 = strVector;
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA7(std::vector<std::string>& strVector)
    {
        AAAA7 = strVector;
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA10.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA10(std::vector<std::string>& strVector)
    {
        AAAA10 = strVector;
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA99.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }

    void DataLumpTest::setAAAA99(std::vector<std::string>& strVector)
    {
        AAAA99 = strVector;
        if (AAAA1.size() > 0 && AAAA2.size() > 0 && AAAA4.size() > 0 && AAAA7.size() > 0 && AAAA10.size() > 0)
        {
            lumpFull = true;
            Logger::getInstance().Debug("DataLumpTestがいっぱいになりました。");
        }
    }