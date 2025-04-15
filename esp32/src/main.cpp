#include <Arduino.h>
#include "RhythmManager.hpp"
#include "RhythmTestRunner.hpp"
#include "SDHelper.hpp"

RhythmManager rhythm;
RhythmTestRunner testRunner;

void setup() {
    Serial.begin(115200);
    delay(300);  // 安定化のための短い待機（必要に応じて）

    if (!SDHelper::begin()) {
        Serial.println("❌ SDカードの初期化に失敗しました。処理を中断します。");
        while (true);  // 無限ループで停止してアクセス回避
    }

    rhythm.begin();
    testRunner.begin(&rhythm);

    testRunner.startTestPattern(1); 
    testRunner.startTestPattern(2); 
    testRunner.startTestPattern(3); 
}

void loop() {
    testRunner.loop();
    rhythm.loop();
}
