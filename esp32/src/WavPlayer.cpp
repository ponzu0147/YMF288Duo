#include <Arduino.h>
#include "WavPlayer.hpp"
#include "RhythmMixer.hpp"
#include <driver/i2s.h>
#include <math.h>

#define I2S_BCLK 6
#define I2S_LRC  7
#define I2S_DOUT 8

uint8_t* WavPlayer::wavData[6] = {nullptr};
size_t WavPlayer::wavSize[6] = {0};
RhythmMixer mixer;

void WavPlayer::setWavBuffers(uint8_t* data[6], size_t size[6]) {
    for (int i = 0; i < 6; ++i) {
        wavData[i] = data[i];
        wavSize[i] = size[i];
    }
}

void WavPlayer::begin() {
    static bool initialized = false;
    if (initialized) return;

    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pins = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);

    mixer.begin();
    Serial.println("✅ I2S 初期化完了");
    initialized = true;
}

bool WavPlayer::hasData(int index) {
    return index >= 0 && index < 6 && wavData[index];
}

void WavPlayer::playFromMemoryStereo(int index, float volL, float volR) {
    if (!hasData(index)) return;

    const int16_t* src = reinterpret_cast<int16_t*>(wavData[index]);
    size_t samples = wavSize[index] / 2;

    size_t fadeLength = (samples < 200) ? samples : 200;
    size_t startFade = samples - fadeLength;

    for (size_t i = 0; i < samples; ++i) {
        float fade = 1.0f;
        if (i >= startFade) {
            fade = float(fadeLength - (i - startFade)) / float(fadeLength);
        }

        int16_t sample = src[i];
        int16_t stereo[2] = {
            (int16_t)(sample * volL * fade),
            (int16_t)(sample * volR * fade)
        };

        size_t written;
        i2s_write(I2S_NUM_0, stereo, sizeof(stereo), &written, portMAX_DELAY);
    }

    int16_t silence[2] = {0};
    for (int i = 0; i < 8; ++i) {
        size_t written;
        i2s_write(I2S_NUM_0, silence, sizeof(silence), &written, portMAX_DELAY);
    }
}

// L:0x01, R:0x02, C:0x03（0.707, 0.707）
// volume = 0〜127
void WavPlayer::playWithPan(int index, uint8_t pan, uint8_t volume) {
    float scale = (127 - volume) / 127.0f;
    float volL = 0.0f, volR = 0.0f;

    switch (pan) {
        case 0x01: volL = 1.0f; volR = 0.0f; break;
        case 0x02: volL = 0.0f; volR = 1.0f; break;
        case 0x03: volL = 0.707f; volR = 0.707f; break;
        default: volL = volR = 0.0f; break;
    }

    volL *= scale;
    volR *= scale;

    playFromMemoryStereo(index, volL, volR);
}

void WavPlayer::mixRhythmOutput(size_t count) {
    mixer.output(count);
}

void WavPlayer::queueRhythm(int index, uint8_t pan, uint8_t volume) {
    if (!hasData(index)) return;
    mixer.setChannel(index, reinterpret_cast<const int16_t*>(wavData[index]), wavSize[index] / 2, pan, volume);
}