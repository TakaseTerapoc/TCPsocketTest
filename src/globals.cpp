#include "globals.hpp"

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestData> gRequestQueue;

// requestQueueのミューテックスです。
std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestData> gRData;