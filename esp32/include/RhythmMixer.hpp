#pragma once
#include <stdint.h>
#include <stddef.h>

struct RhythmChannel {
    const int16_t* sample = nullptr;
    size_t size = 0;
    size_t pos = 0;
    size_t step = 1;
    uint8_t pan = 0x03;   // 0x01 = L, 0x02 = R, 0x03 = C
    uint8_t volume = 0x10;
    bool active = false;
};

class RhythmMixer {
public:
    void begin();
    void setChannel(int index, const int16_t* sample, size_t size, uint8_t pan, uint8_t volume);
    void mix(int16_t* stereoBuffer, size_t count);
    void output(size_t count);

private:
    RhythmChannel channels[6];
};