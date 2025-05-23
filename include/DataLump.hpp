#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include "Logger.hpp"
#include "DataLumpBase.hpp"

using namespace std;

/**
 * @brief DataLumpクラスは、送信間隔ごとのデータの塊を表すクラスです。
 *        各センサーの準備状況や送信データを保持します。
 *        このクラスのメンバーをみてサーバーへ送信すべきかを判断します。
 */
class DataLump{
public:
    // コンストラクタ
    DataLump() = default;

    // シリアルナンバー
    string serialNumber = "";

    // 送信間隔
    int sendIntervalMs = 0;

    // データが満杯かどうか
    bool isSendReady = false;

    // 各センサーの準備状況を保持する
    vector<map<string,bool>> sensorReadyStatus;

    // 送信データ
    vector<map<string,string>> sendData;

    // sensorReadyStatusの状態を確認して、lumpFullを更新する
    void isLumpFull();

    // データをクリアする
    void allClear();
};