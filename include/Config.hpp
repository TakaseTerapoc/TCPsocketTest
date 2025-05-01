// #pragma once

// #include <string>
// #include <iostream>

// /**
//  * @brief PLCとサーバの接続情報を管理するクラスです。
//  */
// class Config
// {
//     private :
//         // PLCのIPアドレス
//         std::string PLCIpAddress;       
        
//         // PLCのポート
//         int PLCPortNumber;  

//         // サーバのIPアドレス
//         std::string serverIpAddress;

//         // サーバのポート
//         int serverPortNumber;           
    
//     public :
//         // コンストラクタ
//         Config(std::string PLCIpAddress, int PLCPortNumber, std::string serverIpAddress, int serverPortNumber);                       
        
//         // 設定ファイル読込む関数
//         void readConfigFile();          
        
//         // ゲッター
//         std::string getPLCIpAddress()
//         {
//             return PLCIpAddress;
//         }

//         int getPLCPortNumber()
//         {
//             return PLCPortNumber;
//         }

//         std::string getserverIpAddress()
//         {
//             return serverIpAddress;
//         }

//         int getserverPortNumber()
//         {
//             return serverPortNumber;
//         }
// };