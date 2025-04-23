#ifndef PLCDATA_HPP
#define PLCDATA_HPP

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

/**
 * @brief PLCから取得したデータと取得時刻、シリアルナンバーを格納するクラスです。
 * 後にサーバへ送信するため、各メンバはchar*型になっています。
*/
class PLCData
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
        PLCData();             

        // マイクロ秒で取得した受信時刻を、ミリ秒までのchar*型に変換します。
        void setReceiptTime();   

        // PLCレスポンスデータを格納します。
        void setResponseData(char* data);    
};

#endif