#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include "PLCRequestData.hpp"
#include "PLCConnectionClient.hpp"

/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

// PLCConnectionClientのインスタンスです。
extern PLCConnectionClient gPLCClient;

// PLCへのリクエストを格納するキューです。
extern std::queue<PLCRequestData> gRequestQueue;

// requestQueueのミューテックスです。
extern std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern std::vector<PLCRequestData> gRData;

//　PLCからの受信完了フラグ
extern bool isReceived;

