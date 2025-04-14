#pragma once

#include <Arduino.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

class AudioWavPlayer {
public:
    void begin();
    void play(const char* path);
    void loop();

private:
    AudioGeneratorWAV* wav = nullptr;
    AudioFileSourceSD* file = nullptr;
    AudioOutputI2S* out = nullptr;
};
