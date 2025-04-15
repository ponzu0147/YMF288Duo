#pragma once
#include "RhythmManager.h"

class RhythmTestRunner {
public:
    void begin(RhythmManager* manager);
    void startTestPattern(uint8_t pattern); // 1, 2, or 3
    void loop();
    bool isRunning() const;

private:
    enum State {
        STATE_IDLE,
        STATE_REPEAT,
        STATE_WAIT
    } state = STATE_IDLE;

    RhythmManager* rhythm = nullptr;
    uint8_t currentPattern = 0;
    uint8_t currentSound = 0;
    uint8_t currentPan = 0;
    uint8_t repeatCount = 0;
    uint8_t playCounter = 0;

    unsigned long lastTime = 0;

    void triggerSound();
};
