#include "DataLump.hpp"

using namespace std;

void DataLump::isLumpFull() {
    for (const auto& sensorStatus : sensorReadyStatus) {
        for (const auto& [sensorID, status] : sensorStatus) {
            lumpFull = true;
            if (!status) {
                lumpFull = false;
            }
        }
    }
}

void DataLump::allClear() {
        lumpFull = false;
        sendData.clear();
        
        // センサーの準備状況をクリア
        for (auto& sensorStatus : sensorReadyStatus) {
            for (auto& [sensorID, status] : sensorStatus) {
                status = false;
            }
        }
}