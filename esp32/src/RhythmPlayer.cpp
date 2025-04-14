#include "RhythmPlayer.hpp"
#include "AudioWavPlayer.hpp"
#include <SD.h>

namespace {
    const char* romPaths[] = {
        "/rom/2608_bd.wav",  // バスドラム
        "/rom/2608_sd.wav",  // スネアドラム
        "/rom/2608_cy.wav",  // シンバル
        "/rom/2608_hh.wav",  // ハイハット
        "/rom/2608_tom.wav", // トム
        "/rom/2608_rim.wav"  // リムショット
    };

    AudioWavPlayer audio;
}

namespace RhythmPlayer {

void begin() {
    audio.begin();
}

void handle(uint8_t addr, uint8_t data) {
    // R#10 = リズムKey Onビット
    if (addr == 0x10) {
        for (int i = 0; i < 6; ++i) {
            if (data & (1 << i)) {
                audio.play(romPaths[i]);
            }
        }
    }
    // 音量制御（R#11〜1D）は未対応（必要に応じて拡張可）
}
}
