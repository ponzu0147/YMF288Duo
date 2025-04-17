#pragma once

class RhythmChannel {
public:
    void begin();
    void loop();
    void playStereoFromMemory(int index, float volL, float volR);
    void playStereo(const char* path, float volL, float volR);
};