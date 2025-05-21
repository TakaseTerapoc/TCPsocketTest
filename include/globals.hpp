#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include "PLCRequestResponseData.hpp"
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
extern queue<PLCRequestResponseData> gRequestQueue;

// requestQueueのミューテックスです。
extern mutex gRequestQueueMutex;

// PLCのリクエストを格納するリストです。
extern vector<PLCRequestResponseData> gRData;

// サーバーへ送信するデータを格納するリストです。
extern vector<vector<string>> gSendData;

// サーバーへ送信するデータを格納するリスト<map>です。
extern vector<vector<map<string, string>>> gSendDataMap;

// サーバーへ送信するデータを格納するリスト２です。
extern vector<DataLumpBase> gSendData2;

// サーバーへ送信するDataLumpBaseを格納するリストです。
extern vector<DataLumpBase*> gDataLumps;

// gSendDataのミューテックスです。
extern mutex gSendDataMutex;

// シグナル制御・管理するためのオブジェクトです。
extern atomic<bool> gShouldExit;
extern AppController* gAppInstance;

// DataLumpのvectorです。
extern vector<DataLump> gDataLump;
