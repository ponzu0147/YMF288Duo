#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "S98Player.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    // SPI初期化 (VSPI: SCK=18, MISO=19, MOSI=23)
    SPI.begin(18, 19, 23);

    // SDカード初期化
    if (!SD.begin(5)) {
        Serial.println("[ERROR] SDカード初期化失敗");
        while (1);
    }
    Serial.println("[OK] SDカード初期化成功");

    // S98再生開始
    if (!S98Player::begin("/test.s98")) {
        Serial.println("[ERROR] S98ファイルが開けません");
        while (1);
    }
    Serial.println("[OK] S98ファイル初期化成功");
}

void loop() {
    S98Player::update();
    delay(1);  // 処理負荷を軽減（適宜調整）
}
