// SDHelper.h
#ifndef SDHELPER_H
#define SDHELPER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "config.h"

namespace SDHelper {
    const int SD_CS_PIN = PIN_SD_CS;  // GPIO5（CS）

    inline bool begin() {
        SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI);
        if (!SD.begin(SD_CS_PIN)) {
            Serial.println("[SD] 初期化失敗！");
            return false;
        }
        Serial.println("[SD] 初期化成功！");
        return true;
    }

    inline File open(const char* path, const char* mode = FILE_READ) {
        return SD.open(path, mode);
    }

    inline bool exists(const char* path) {
        return SD.exists(path);
    }

    inline void end() {
        SD.end();
    }
}

#endif
