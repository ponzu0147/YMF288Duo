#include "RhythmTestRunner.hpp"
#include <Arduino.h>

void RhythmTestRunner::begin(RhythmManager* manager) {
    rhythm = manager;
    state = STATE_IDLE;
}

void RhythmTestRunner::startTestPattern(uint8_t pattern) {
    currentPattern = pattern;
    currentSound = 0;
    currentPan = 0;
    playCounter = 0;
    lastTime = millis();

    switch (pattern) {
        case 1: repeatCount = 1; break;
        case 2: repeatCount = 2; break;
        case 3: repeatCount = 4; break;
        default: repeatCount = 1; break;
    }

    state = STATE_REPEAT;
}

bool RhythmTestRunner::isRunning() const {
    return state != STATE_IDLE;
}

void RhythmTestRunner::loop() {
    if (state == STATE_IDLE) return;

    unsigned long now = millis();

    if (state == STATE_REPEAT) {
        if (now - lastTime >= 200) {
            triggerSound();
            playCounter++;
            lastTime = now;

            if (playCounter >= repeatCount) {
                playCounter = 0;
                state = STATE_WAIT;
                lastTime = now;
            }
        }
    } else if (state == STATE_WAIT) {
        if (now - lastTime >= 1000) {
            currentPan++;
            if (currentPan > 2) {
                currentPan = 0;
                currentSound++;
                if (currentSound > 5) {
                    state = STATE_IDLE;
                    return;
                }
            }
            state = STATE_REPEAT;
        }
    }
}

void RhythmTestRunner::triggerSound() {
    float vol = 1.0f;
    float volL = 0.0f, volR = 0.0f;

    switch (currentPan) {
        case 0: volL = vol; break;
        case 1: volL = volR = vol; break;
        case 2: volR = vol; break;
    }

    const char* path = nullptr;
    switch (currentSound) {
        case 0: path = "/WAV/2608_BD.WAV"; break;
        case 1: path = "/WAV/2608_SD.WAV"; break;
        case 2: path = "/WAV/2608_HH.WAV"; break;
        case 3: path = "/WAV/2608_TOM.WAV"; break;
        case 4: path = "/WAV/2608_RIM.WAV"; break;
        case 5: path = "/WAV/2608_TOP.WAV"; break;
    }

    if (path) {
        rhythm->playDirect(path, volL, volR);
    }
}
