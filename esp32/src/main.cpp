#include <Arduino.h>
#include "RhythmManager.hpp"
#include "RhythmTestRunner.hpp"

RhythmManager rhythm;
RhythmTestRunner testRunner;

void setup() {
    Serial.begin(115200);
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
