#include "DataLump.hpp"

void DataLump::isLumpFull() {
    for (const auto& sensorStatus : sensorReadyStatus) {
        for (const auto& [sensorID, status] : sensorStatus) {
            isSendReady = true;
            if (!status) {
                isSendReady = false;
            }
        }
    }
}

void DataLump::allClear() {
        isSendReady = false;
        sendData.clear();
        
        // センサーの準備状況をクリア
        for (auto& sensorStatus : sensorReadyStatus) {
            for (auto& [sensorID, status] : sensorStatus) {
                status = false;
            }
        }
}