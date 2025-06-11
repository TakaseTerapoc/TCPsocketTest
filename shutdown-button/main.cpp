#include <gpiod.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <cstdlib>

const char* chip_path = "/dev/gpiochip0"; // GPIOチップ名
constexpr unsigned int BUTTON_LINE_NUM = 3; // GPIO3 = physical pin 5
constexpr int LONG_PRESS_MS = 3000; // 長押し判定時間（ミリ秒）

int main() {
    // GPIOチップを開く
    struct gpiod_chip *chip = gpiod_chip_open(chip_path);
    if (!chip) {
        std::cerr << "チップの設定が正しくできませんでした。\n";
        return 1;
    }

    // GPIOライン取得（GPIO3）
    gpiod_line* line = gpiod_chip_get_line(chip, BUTTON_LINE_NUM);
    if (!line) {
        std::cerr << "GPIO3の設定が正しくできませんでした。\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // プルアップ入力として要求
    // "shutdownButton"文字列はgpioinfoコマンドで表示される。
    if (gpiod_line_request_input_flags(line, "shutdownButton", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) < 0) {
        std::cerr << "プルアップ設定が正しくできませんでした。\n";
        gpiod_chip_close(chip);
        return 1;
    }

    auto last_low = std::chrono::steady_clock::now();
    bool pressed = false;

    // シャットダウンボタンが最初に押下されてから、３秒経つまでループ
    while (true) {
        int v = gpiod_line_get_value(line);
        if (v < 0) {
            std::cerr << "GPIO3ピンが正しく読み込めませんでした。\n";
            break;
        }

        if (v == 0) { // ボタン押下（LOW）
            
            if (!pressed) {
                pressed = true;
                last_low = std::chrono::steady_clock::now();
            } else {
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - last_low).count();
                    
                if (duration >= LONG_PRESS_MS) {
                    std::cout << "シャットダウンします。\n";
                    system("wall 'シャットダウンしています。'");
                    system("sudo systemctl stop NAKANISHI_IOT_SYSTEM_PROTTYPE.service");
                    system("sudo shutdown -h now");
                    break;
                }
            }
        } else {
            pressed = false;
        }

        usleep(100 * 1000); // 100ms
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}
