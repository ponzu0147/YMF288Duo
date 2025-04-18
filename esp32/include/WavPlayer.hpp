#pragma once
#include <stdint.h>
#include <stddef.h>

class WavPlayer {
public:
    static void begin();
    static bool hasData(int index);
    static void setWavBuffers(uint8_t* data[6], size_t size[6]);
    static void playFromMemoryStereo(int index, float volL, float volR);
    static void playWithPan(int index, uint8_t pan, uint8_t volume);
    static void queueRhythm(int index, uint8_t pan, uint8_t volume);
    static void mixRhythmOutput(size_t count);

private:
    static uint8_t* wavData[6];  // ← 追加！
    static size_t wavSize[6];    // ← 追加！
};
