#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

/**
 * @brief PLCから取得したデータと取得時刻、シリアルナンバーを格納するクラスです。
 * 後にサーバへ送信するため、各メンバはchar*型になっています。
*/
class PLCResponseData
{
    private:
        // PLCレスポンスデータの受信時間です。
        std::string receiptTime;        
        
        // PLCレスポンスデータです。
        std::string responseData;   

        // サーバに送信する際のデータです。
        char* sendData;                 

    public:
        // コンストラクタです。
        PLCResponseData();             

        // マイクロ秒で取得した受信時刻を、ミリ秒までのchar*型に変換します。
        void setReceiptTime();   

        // PLCレスポンスデータを格納します。
        void setResponseData(char* data);    
};