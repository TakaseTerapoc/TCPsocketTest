#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include "PLCTransactionData.hpp"
#include "DataLumpBase.hpp"
#include "AppController.hpp"
#include "PLCConnectionClient.hpp"
#include "DataLump.hpp"

using namespace std;

/**
 * @brief グローバルオブジェクトを格納するヘッダーファイルです。
 */

class AppController;

// PLCへのリクエストを格納するキューです。
extern queue<PLCTransactionData> gRequestQueue;

// requestQueueのミューテックスです。
extern mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern vector<PLCTransactionData> gRData;

// サーバーへ送信するデータを格納するリスト<map>です。
extern vector<vector<map<string, string>>> gSendDataMap;

// gSendDataのミューテックスです。
extern mutex gSendDataMutex;

// シグナル制御・管理するためのオブジェクトです。
extern atomic<bool> gShouldExit;
extern AppController* gAppInstance;

// DataLumpのvectorです。
extern vector<DataLump> gDataLump;
