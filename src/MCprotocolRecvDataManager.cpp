#include "MCprotocolRecvDataManager.hpp"

namespace FX3UC
{
    // シングルトンインスタンス取得
    MCprotocolRecvDataManager& MCprotocolRecvDataManager::getInstance() {
        static MCprotocolRecvDataManager instance;
        return instance;
    }

    vector<map<string,string>> MCprotocolRecvDataManager::convertResponseDataToSendData(char* text, int len, PLCTransactionData& req) {
        string responseData;
        string format;
        vector<map<string,string>> sendData;

        for (int i = 0; i < len; ++i)
        {
            if (i > 1)
            {
                responseData += Utilities::convertByteToHexString(text[i]);

            }
            else
            {
                format += Utilities::convertByteToHexString(text[i]);
            }
        }

        Logger::getInstance().Debug("【シリアルナンバー】" + req.serialNumber + "【フォーマット】"+ format + "【受信データ(16進数)】" + responseData);

        for (int i = 0; i < req.mapdata.size(); i++)
        {
            string data;
            if (req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("M") 
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("X")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("Y")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("S")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TS")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CS")
            )
            {
                if (i == 0)
                {
                    data = responseData.substr(0, 1);
                }
                else
                {
                    int startPosition = stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"]);
                    data = responseData.substr(startPosition, 1);
                }
            }
            else if (req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("D")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("R")
                || req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("TN")
            )
            {
                if (i == 0)
                {
                    data = Utilities::convertDecimalString(Utilities::swapString16Bit(responseData.substr(0, 4)));

                }
                else
                {
                    int startPosition = (stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"])) * 4;
                    data = Utilities::convertDecimalString(Utilities::swapString16Bit(responseData.substr(startPosition, 4)));
                }
            }
            else if (req.deviceCode == MCprotocolConfigData::deviceCodeToASCIIMap.at("CN")
            )
            {
                if (i == 0)
                {
                    data = Utilities::convertDecimalString(Utilities::swapString32Bit(responseData.substr(0, 8)));
                }
                else
                {
                    int startPosition = (stoi(req.mapdata[i]["ASCII"]) - stoi(req.mapdata[i - 1]["ASCII"])) * 8;
                    data = Utilities::convertDecimalString(Utilities::swapString32Bit(responseData.substr(startPosition, 8)));
                }
            }
            else
            {
                Logger::getInstance().Error("convertResponseDataToSendData：デバイスコードが不正です。");
                exit(1);
            }
            req.mapdata[i]["data"] = data;
        }
        sendData = req.mapdata;

        return sendData;
    }
}
