#include "globals.hpp"

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestResponseData> gRequestQueue;

// gRequestQueueのミューテックスです。
std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestResponseData> gRData;

// サーバーへ送信するデータを格納するリストです。
std::vector<std::vector<std::string>> gSendData;

// gSendDataのミューテックスです。
std::mutex gSendDataMutex;