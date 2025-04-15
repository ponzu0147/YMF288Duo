#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "config.h"

namespace SDHelper {
    const int SD_CS_PIN = PIN_SD_CS;  // GPIO（CSピン）

    bool begin();
    File open(const char* path, const char* mode = FILE_READ);
    bool exists(const char* path);
    void end();
}
