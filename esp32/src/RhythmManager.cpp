#include "RhythmManager.hpp"
#include <string.h>

void RhythmManager::begin() {
    static bool i2sInitialized = false;
    if (!i2sInitialized) {
        bd.begin();           // ← WavPlayer::begin() 呼び出し
        i2sInitialized = true;
    }
    // 他のchはbegin不要（I2S共有）
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

    if (keyOnBits & 0x01) { decodeVolume(rimReg, l, r); rim.playStereo("/WAV/2608_RIM.WAV", l, r); }
    if (keyOnBits & 0x02) { decodeVolume(tomReg, l, r); tom.playStereo("/WAV/2608_TOM.WAV", l, r); }
    if (keyOnBits & 0x04) { decodeVolume(hhReg, l, r);  hh.playStereo("/WAV/2608_HH.WAV", l, r); }
    if (keyOnBits & 0x08) { decodeVolume(topReg, l, r); top.playStereo("/WAV/2608_TOP.WAV", l, r); }
    if (keyOnBits & 0x10) { decodeVolume(sdReg, l, r);  sd.playStereo("/WAV/2608_SD.WAV", l, r); }
    if (keyOnBits & 0x20) { decodeVolume(bdReg, l, r);  bd.playStereo("/WAV/2608_BD.WAV", l, r); }
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

void RhythmManager::decodeVolume(uint8_t reg, float& outL, float& outR) const {
    const float MASTER_VOLUME = 0.01f;  // 全体スケーリング係数

    uint8_t pan = (reg >> 5) & 0x07;
    float base = (31 - (reg & 0x1F)) / 31.0f;
    float total = (31 - totalLevel) / 31.0f;
    float vol = base * total * MASTER_VOLUME;

    switch (pan) {
        case 0x01: outL = vol; outR = 0.0f; break;
        case 0x02: outL = 0.0f; outR = vol; break;
        default:   outL = outR = vol; break;
    }
}

