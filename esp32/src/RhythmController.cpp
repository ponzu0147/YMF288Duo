#include "RhythmController.hpp"
#include "RhythmPlayer.hpp"
#include "Ymf288Driver.hpp"
#include "config.h"

namespace {
    bool hardwareAvailable = false;

    bool detectHardware() {
        // /COMピンなどでYMF288の存在確認（例：LOWなら接続）
        pinMode(PIN_HW_DETECT, INPUT_PULLUP);
        return digitalRead(PIN_HW_DETECT) == LOW;
    }
}

namespace RhythmController {

void begin() {
    hardwareAvailable = detectHardware();

    if (hardwareAvailable) {
        Ymf288Driver::begin();
    } else {
        RhythmPlayer::begin();
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
