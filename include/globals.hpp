#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include "PLCRequestResponseData.hpp"
#include "PLCConnectionClient.hpp"
#include "DataLumpBase.hpp"

/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

// PLCへのリクエストを格納するキューです。
extern std::queue<PLCRequestResponseData> gRequestQueue;

// requestQueueのミューテックスです。
extern std::mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern std::vector<PLCRequestResponseData> gRData;

// サーバーへ送信するデータを格納するリストです。
extern std::vector<std::vector<std::string>> gSendData;

// サーバーへ送信するデータを格納するリスト２です。
extern std::vector<DataLumpBase> gSendData2;

// gSendDataのミューテックスです。
extern std::mutex gSendDataMutex;

