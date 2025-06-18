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
atomic<bool> gShouldExit = false;
AppController* gAppInstance = nullptr;

// DataLumpのvectorです。
vector<DataLump> gDataLump;

// // サーバーへ送信するデータを格納するリスト(map)です。
// vector<vector<map<string, string>>> gSendDataMap;

// サーバーへ送信するデータを格納するリスト(vectorString)です。
vector<string> gSendDataVectorStr;

// PLCとの通信が再開した時のフラグです。
atomic<bool> gPLCconnectFlag = true;

// キューを空にするためのフラグです。
atomic<bool> gClearQueueFlag = false;

// アドレスコードの長さ
const int ADDRESSLENGTH = 5; 


// 再送信データを貯めるキュー
BlockingVector<char*> gResendVector;


// 再送信データを貯めるヴェクタ
vector<char*> gResendVectorData;

// ServerSendDataBuilderのvector
vector<ServerSendDataBuilder> gServerSendDataBuilderVector;

// 再送信データのミューテックス
mutex gResendVectorMutex;

BlockingVector<ServerSendDataBuilder> gResendVectorServerSendDataBuilder;

// 待機するためのcondition_variable
condition_variable gcv;