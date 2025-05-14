#pragma once

class DataLumpBase {
public:
    virtual ~DataLumpBase() = default;

    bool isLumpFull() const { return lumpFull; }
    void setLumpFull(bool val) { lumpFull = val; }

protected:
    bool lumpFull = false;
};