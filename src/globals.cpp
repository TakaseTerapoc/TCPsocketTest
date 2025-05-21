#include "globals.hpp"

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestResponseData> gRequestQueue;

// gRequestQueueのミューテックスです。
std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestResponseData> gRData;

// サーバーへ送信するデータを格納するリストです。
std::vector<std::vector<std::string>> gSendData;

// サーバーへ送信するDataLumpBaseを格納するリストです。
std::vector<DataLumpBase*> gDataLumps;

// gSendDataのミューテックスです。
std::mutex gSendDataMutex;

// シグナル制御・管理するための変数です。
std::atomic<bool> gShouldExit = false;
AppController* gAppInstance = nullptr;

// DataLumpのvectorです。
std::vector<DataLump> gDataLump;

// サーバーへ送信するデータを格納するリスト(map)です。
vector<vector<map<string, string>>> gSendDataMap;