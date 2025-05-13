#include "globals.hpp"

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestResponseData> gRequestQueue;

// requestQueueのミューテックスです。
std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestResponseData> gRData;