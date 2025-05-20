#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include "PLCRequestResponseData.hpp"
#include "DataLumpBase.hpp"
#include "AppController.hpp"
#include "PLCConnectionClient.hpp"


/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

 class AppController;

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

// サーバーへ送信するDataLumpBaseを格納するリストです。
extern std::vector<DataLumpBase*> gDataLumps;

// gSendDataのミューテックスです。
extern std::mutex gSendDataMutex;

// シグナル制御・管理するためのオブジェクトです。
extern std::atomic<bool> gShouldExit;
extern AppController* gAppInstance;
