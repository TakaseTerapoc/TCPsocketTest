#include "globals.hpp"

// PLCConnectionClientのインスタンスです。
PLCConnectionClient gPLCClient;

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestData> gRequestQueue;

// requestQueueのミューテックスです。
std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestData> gRData;

bool isReceived;