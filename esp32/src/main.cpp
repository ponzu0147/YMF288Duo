#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "SDHelper.hpp"
#include "WavInitialize.hpp"
#include "RhythmManager.hpp"

RhythmManager rhythm;

void setup() {
    Serial.begin(115200);
    delay(300);

    if (!SDHelper::begin()) {
        Serial.println("❌ SDカード初期化失敗");
        while (1);
    }

    listWavFiles();

    if (!initializeWavFiles()) {
        Serial.println("❌ WAV読み込み失敗");
        while (1);
    }

    rhythm.begin();

    // テスト再生：すべての音を順にセンター定位で鳴らす
    for (int i = 0; i < 6; ++i) {
        Serial.printf("🔊 play rhythm[%d]\n", i);
        rhythm.setVolume(0x11 + i, 0x10); // 標準ボリューム
        rhythm.play(1 << i);              // 単独ビットON
        delay(600);
    }
}

void loop() {
    rhythm.loop();  // I2S出力
}