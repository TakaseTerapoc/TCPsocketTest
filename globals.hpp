#pragma once

#include <queue>
#include "PLCRequestData.hpp"

/**
 * @brief グローバル変数を格納するヘッダーファイルです。
 */
extern std::queue<PLCRequestData> requestQueue;