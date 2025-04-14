#include "RhythmController.hpp"
#include "RhythmPlayer.hpp"
#include "Ymf288Driver.hpp"

namespace {
    bool hardwareAvailable = false;

    bool detectHardware() {
        // 例: YMF288の/COMピンなどを読み取り、接続を確認（ダミー実装）
        pinMode(34, INPUT_PULLUP);
        return digitalRead(34) == LOW;  // 実装に応じて調整
    }
}

namespace RhythmController {

void begin() {
    hardwareAvailable = detectHardware();

    if (hardwareAvailable) {
        Ymf288Driver::begin();
    } else {
        RhythmPlayer::begin();  // WAV再生の初期化
    }
}

bool isHardwareConnected() {
    return hardwareAvailable;
}

void handle(uint8_t addr, uint8_t data) {
    if (hardwareAvailable) {
        Ymf288Driver::writeRegister(addr, data);
    } else {
        RhythmPlayer::handle(addr, data);
    }
}
}
