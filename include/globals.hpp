#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include "PLCRequestResponseData.hpp"
#include "PLCConnectionClient.hpp"

/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

// PLCへのリクエストを格納するキューです。
extern std::queue<PLCRequestResponseData> gRequestQueue;

// requestQueueのミューテックスです。
extern std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern std::vector<PLCRequestResponseData> gRData;

