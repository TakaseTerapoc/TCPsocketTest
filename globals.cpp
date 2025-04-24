#include "globals.hpp"

// PLCへのリクエストを格納するキューです。
std::queue<PLCRequestData> requestQueue;

// requestQueueのミューテックスです。
std::mutex requestQueueMutex;

// PLCのリクエストを格納するリストです。
std::vector<PLCRequestData> rdata;