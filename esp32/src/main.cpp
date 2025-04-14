#include <Arduino.h>
#include <FS.h>
#include <SD.h>

#include "S98TimerPlayer.hpp"
#include "AdpcmPlayer.hpp"
#include "RhythmController.hpp"
#include "config.h"  // ESP32-S3 ピン定義

S98TimerPlayer player;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println("YMF288Duo S98 TimerPlayer 起動");

    if (!SD.begin(PIN_SD_CS)) {
        Serial.println("SDカード初期化失敗");
        while (1);
    }

    File s98File = SD.open("/music/test.s98");
    if (!s98File) {
        Serial.println("S98ファイル読み込み失敗");
        while (1);
    }

    AdpcmPlayer::begin();
    RhythmController::begin();

    player.begin(s98File);
    player.start();

    Serial.println("S98再生開始（タイマー割り込み）");
}

void loop() {
    // メインループでは何もしない（非ブロッキング再生）
}
