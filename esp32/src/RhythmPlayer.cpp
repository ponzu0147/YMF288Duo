#include "RhythmPlayer.hpp"
#include <SD.h>
#include <Audio.h>  // Arduino用の軽量WAV再生ライブラリなどに差し替え可

namespace {
    const char* romPaths[] = {
        "/rom/2608_bd.wav",  // バスドラム
        "/rom/2608_sd.wav",  // スネアドラム
        "/rom/2608_cy.wav",  // シンバル
        "/rom/2608_hh.wav",  // ハイハット
        "/rom/2608_tom.wav", // トム
        "/rom/2608_rim.wav"  // リムショット
    };

    File wavFile;
    Audio audio;  // 仮想WAV再生クラス（PCM出力付き）
}

namespace RhythmPlayer {

void begin() {
    // 初期化処理など（必要に応じて）
}

void handle(uint8_t addr, uint8_t data) {
    // リズム音のKey On（0x10 = R#10 = RKonビット）
    if (addr == 0x10) {
        for (int i = 0; i < 6; ++i) {
            if (data & (1 << i)) {
                if (wavFile) wavFile.close();
                wavFile = SD.open(romPaths[i]);
                if (wavFile) {
                    audio.begin();
                    audio.play(wavFile);
                }
            }
        }
    }
    // 音量レジスタ（0x11〜1D）なども対応可能（将来的に）
}
}
