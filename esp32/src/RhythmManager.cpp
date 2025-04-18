#include <Arduino.h>
#include "RhythmManager.hpp"
#include "WavPlayer.hpp"

void RhythmManager::begin() {
    WavPlayer::begin();
}

void RhythmManager::loop() {
    WavPlayer::mixRhythmOutput(256);
}

void RhythmManager::play(uint8_t keyOnBits) {
    for (int i = 0; i < 6; ++i) {
        if (!(keyOnBits & (1 << i))) continue;

        float volL = 0.0f, volR = 0.0f;
        decodeVolume(i, volL, volR);
        uint8_t pan = (panMask[i] & 0x03);
        uint8_t vol = volumeTable[i];
        WavPlayer::queueRhythm(i, pan, vol);
    }
}

void RhythmManager::setVolume(uint8_t addr, uint8_t data) {
    if (addr >= 0x12 && addr <= 0x17) {
        volumeTable[addr - 0x12] = data;
    } else if (addr == 0x11) {
        masterVolume = data;
    }
}

void RhythmManager::decodeVolume(int index, float& outL, float& outR) const {
    uint8_t vol = volumeTable[index];
    uint8_t pan = panMask[index];
    float scale = (127 - vol) / 127.0f;

    switch (pan) {
        case 0x01: outL = scale; outR = 0.0f; break;
        case 0x02: outL = 0.0f; outR = scale; break;
        case 0x03: outL = outR = scale * 0.707f; break;
        default:   outL = outR = 0.0f; break;
    }
}

// 空のダミー実装（RhythmTestRunner 対応用）
void RhythmManager::playDirect(const char* filepath, float volL, float volR) {
    Serial.printf("playDirect is not implemented: %s\n", filepath);
}