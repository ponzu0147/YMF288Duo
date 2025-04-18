#include "RhythmMixer.hpp"
#include <string.h>
#include <driver/i2s.h>

void RhythmMixer::begin() {
    // 初期化が必要ならここに書く
}

void RhythmMixer::setChannel(int index, const int16_t* sample, size_t size, uint8_t pan, uint8_t volume) {
    if (index < 0 || index >= 6) return;
    channels[index].sample = sample;
    channels[index].size = size;
    channels[index].pos = 0;
    channels[index].step = 1;
    channels[index].pan = pan;
    channels[index].volume = volume;
    channels[index].active = true;
}

void RhythmMixer::mix(int16_t* stereoBuffer, size_t count) {
    memset(stereoBuffer, 0, sizeof(int16_t) * count * 2);  // L/R交互

    for (int ch = 0; ch < 6; ++ch) {
        RhythmChannel& r = channels[ch];
        if (!r.active || !r.sample) continue;

        for (size_t i = 0; i < count && r.pos < r.size; ++i, r.pos += r.step) {
            int16_t s = r.sample[r.pos];
            int32_t v = ((127 - r.volume) * s) >> 7;

            if (r.pan & 0x01) stereoBuffer[i * 2]     += v; // L
            if (r.pan & 0x02) stereoBuffer[i * 2 + 1] += v; // R
        }

        if (r.pos >= r.size) {
            r.active = false;  // 再生終了
        }
    }
}

void RhythmMixer::output(size_t count) {
    int16_t buffer[count * 2];
    mix(buffer, count);

    size_t written;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &written, portMAX_DELAY);
}