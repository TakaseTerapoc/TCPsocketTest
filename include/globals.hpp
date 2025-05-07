#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include "PLCRequestData.hpp"
#include "PLCConnectionClient.hpp"

/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

// PLCへのリクエストを格納するキューです。
extern std::queue<PLCRequestData> gRequestQueue;

// requestQueueのミューテックスです。
extern std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern std::vector<PLCRequestData> gRData;

