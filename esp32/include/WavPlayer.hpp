#pragma once
#include <Arduino.h>

class WavPlayer {
public:
    static void begin();
    static void playFromMemory(int index);
    static void setWavBuffers(uint8_t* data[6], size_t size[6]);
    static bool hasData(int index);
    static void playFromMemoryStereo(int index, float volL, float volR);

private:
    static uint8_t* wavData[6];
    static size_t wavSize[6];
};