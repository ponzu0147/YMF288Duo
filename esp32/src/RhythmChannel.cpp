#include "RhythmChannel.hpp"
#include "WavPlayer.hpp"

void RhythmChannel::begin() {}

void RhythmChannel::loop() {}

void RhythmChannel::playStereoFromMemory(int index, float volL, float volR) {
    if (WavPlayer::hasData(index)) {
        WavPlayer::playFromMemoryStereo(index, volL, volR);
    }
}

void RhythmChannel::playStereo(const char* path, float volL, float volR) {
    // ダミー関数。必要であればSD再生処理をここに
}