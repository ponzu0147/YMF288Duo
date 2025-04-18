#pragma once
#include <stdint.h>

class RhythmManager {
public:
    void begin();
    void loop();
    void play(uint8_t keyOnBits);
    void setVolume(uint8_t addr, uint8_t data);
    void playDirect(const char* filepath, float volL, float volR); // 追加

private:
    void decodeVolume(int index, float& outL, float& outR) const;

    uint8_t volumeTable[6] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
    uint8_t panMask[6]     = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
    uint8_t masterVolume   = 0x00;
};