#pragma once
#include <Arduino.h>
#include <FS.h>

class WavPlayer {
public:
    void begin();
    bool playStereo(const char* filepath, float volL, float volR);
    void stop();
    void loop();
    bool isPlaying() const;

private:
    File wavFile;
    bool playing = false;
    float volumeL = 0.3f;
    float volumeR = 0.3f;
};
