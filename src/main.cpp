#include <iostream>
#include <string>
#include "AppController.hpp"

int main() 
{
    AppController app;
    app.run();
    Logger::getInstance().Debug("終了要求を検出したため、接続処理を中止します。ログを出力してから終了します。");
    Logger::getInstance().Flush();
    std::cout << "アプリケーションが正常に終了しました。" << std::endl;
    return 0;
}