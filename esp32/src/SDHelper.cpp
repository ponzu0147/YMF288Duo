#include "SDHelper.hpp"

namespace SDHelper {
    bool begin() {
        SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI);
        if (!SD.begin(SD_CS_PIN)) {
            Serial.println("[SD] 初期化失敗！");
            return false;
        }
        Serial.println("[SD] 初期化成功！");
        return true;
    }

    File open(const char* path, const char* mode) {
        return SD.open(path, mode);
    }

    bool exists(const char* path) {
        return SD.exists(path);
    }

    void end() {
        SD.end();
    }
}
