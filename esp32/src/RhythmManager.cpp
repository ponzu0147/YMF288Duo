#include "RhythmManager.hpp"
#include <string.h>

void RhythmManager::begin() {
    static bool i2sInitialized = false;
    if (!i2sInitialized) {
        bd.begin();
        i2sInitialized = true;
    }
}

void RhythmManager::loop() {
    bd.loop();
    sd.loop();
    hh.loop();
    tom.loop();
    rim.loop();
    top.loop();
}

void RhythmManager::play(uint8_t keyOnBits) {
    float l, r;

    if (keyOnBits & 0x01) { decodeVolume(rimReg, l, r); rim.playStereoFromMemory(4, l, r); }
    if (keyOnBits & 0x02) { decodeVolume(tomReg, l, r); tom.playStereoFromMemory(3, l, r); }
    if (keyOnBits & 0x04) { decodeVolume(hhReg,  l, r); hh.playStereoFromMemory(2, l, r); }
    if (keyOnBits & 0x08) { decodeVolume(topReg, l, r); top.playStereoFromMemory(5, l, r); }
    if (keyOnBits & 0x10) { decodeVolume(sdReg,  l, r); sd.playStereoFromMemory(1, l, r); }
    if (keyOnBits & 0x20) { decodeVolume(bdReg,  l, r); bd.playStereoFromMemory(0, l, r); }
}

void RhythmManager::setVolume(uint8_t addr, uint8_t data) {
    switch (addr) {
        case 0x11: totalLevel = data & 0x1F; break;
        case 0x12: bdReg = data; break;
        case 0x13: sdReg = data; break;
        case 0x14: tomReg = data; break;
        case 0x15: topReg = data; break;
        case 0x16: hhReg = data; break;
        case 0x17: rimReg = data; break;
    }
}

void RhythmManager::playDirect(const char* filepath, float volL, float volR) {
    if (strstr(filepath, "BD")) bd.playStereo(filepath, volL, volR);
    else if (strstr(filepath, "SD")) sd.playStereo(filepath, volL, volR);
    else if (strstr(filepath, "HH")) hh.playStereo(filepath, volL, volR);
    else if (strstr(filepath, "TOM")) tom.playStereo(filepath, volL, volR);
    else if (strstr(filepath, "RIM")) rim.playStereo(filepath, volL, volR);
    else if (strstr(filepath, "TOP")) top.playStereo(filepath, volL, volR);
}