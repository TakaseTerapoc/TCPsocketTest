#include <iostream>
#include <string>
#include "AppController.hpp"

int main() 
{
    AppController app;
    app.run();
    Logger::getInstance().Debug("ログを出力してから終了します。");
    Logger::getInstance().Flush();
    return 0;
}