#pragma once

#include "WavPlayer.hpp"

class RhythmManager {
public:
    void begin();
    void loop();
    void play(uint8_t keyOnBits);
    void setVolume(uint8_t addr, uint8_t data);
    void playDirect(const char* filepath, float volL, float volR);

private:
    WavPlayer bd, sd, hh, tom, rim, top;

    uint8_t totalLevel = 0;
    uint8_t bdReg = 0, sdReg = 0, hhReg = 0;
    uint8_t tomReg = 0, rimReg = 0, topReg = 0;

    void decodeVolume(uint8_t reg, float& outL, float& outR) const;
};
