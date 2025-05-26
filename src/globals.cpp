#include "globals.hpp"

// PLCのリクエストを格納するリストです。
vector<PLCTransactionData> gRData;

// PLCへのリクエストを格納するキューです。
deque<PLCTransactionData> gRequestQueue;

// gRequestQueueのミューテックスです。
mutex gRequestQueueMutex;

// gSendDataのミューテックスです。
mutex gSendDataMutex;

// シグナル制御・管理するための変数です。
// atomic<bool> gShouldExit = false;
AppController* gAppInstance = nullptr;

// DataLumpのvectorです。
vector<DataLump> gDataLump;

// サーバーへ送信するデータを格納するリスト(map)です。
vector<vector<map<string, string>>> gSendDataMap;

// PLCとの通信が再開した時のフラグです。
atomic<bool> gPLCconnectFlag = true;

// キューを空にするためのフラグです。
atomic<bool> gClearQueueFlag = false;