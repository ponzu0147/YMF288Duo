#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "WavInitialize.hpp"
#include "WavPlayer.hpp"

int currentSound = 0;
int currentPan = 0;
int repeatCount = 0;
unsigned long lastTrigger = 0;

const int maxSounds = 6;
const char* soundNames[6] = { "BD", "SD", "HH", "TOM", "RIM", "TOP" };

void setup() {
    Serial.begin(115200);
    delay(300);

    if (!SD.begin()) {
        Serial.println("❌ SDカード初期化失敗");
        while (1);
    }

    listWavFiles();

    if (!initializeWavFiles()) {
        Serial.println("❌ WAV読み込み失敗");
        while (1);
    }

    WavPlayer::begin();
    delay(500);
    Serial.println("🔁 テスト再生開始：L→C→R 各音4回ずつ");
}

void loop() {
    if (millis() - lastTrigger >= 800) {  // 音間インターバル（ms）
        float volL = 0.0f;
        float volR = 0.0f;

        switch (currentPan) {
            case 0: volL = 1.0f; volR = 0.0f; break; // L
            case 1: volL = 0.707f; volR = 0.707f; break; // C
            case 2: volL = 0.0f; volR = 1.0f; break; // R
        }

        Serial.printf("🔊 %s %s (%d/4)\n", soundNames[currentSound], (currentPan == 0 ? "L" : currentPan == 1 ? "C" : "R"), repeatCount + 1);
        WavPlayer::playFromMemoryStereo(currentSound, volL, volR);

        repeatCount++;
        if (repeatCount >= 4) {
            repeatCount = 0;
            currentPan++;
            if (currentPan >= 3) {
                currentPan = 0;
                currentSound++;
                if (currentSound >= maxSounds) {
                    Serial.println("✅ テスト完了。ループ停止。");
                    while (1);  // 停止
                }
            }
        }

        lastTrigger = millis();
    }
}
