#include "WavPlayer.hpp"
#include <SD.h>
#include <driver/i2s.h>

void WavPlayer::begin() {
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

    i2s_pin_config_t pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 22,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

bool WavPlayer::play(const char* filepath) {
    return playStereo(filepath, 1.0f, 1.0f); // モノラル → ステレオ中央定位として再生
}

bool WavPlayer::playStereo(const char* filepath, float volL, float volR) {
    stop();

    wavFile = SD.open(filepath);
    if (!wavFile) return false;

    uint8_t skip[44];
    wavFile.read(skip, 44); // WAVヘッダーをスキップ

    volumeL = volL;
    volumeR = volR;
    playing = true;
    return true;
}

void WavPlayer::stop() {
    if (playing) {
        wavFile.close();
        playing = false;
    }
}

bool WavPlayer::isPlaying() const {
    return playing;
}

void WavPlayer::loop() {
    if (!playing) return;

    const size_t samples = 256;
    int16_t monoBuf[samples];
    int16_t stereoBuf[samples * 2];

    size_t bytesRead = wavFile.read((uint8_t*)monoBuf, sizeof(monoBuf));
    if (bytesRead == 0) {
        stop();
        return;
    }

    size_t count = bytesRead / 2;
    for (size_t i = 0; i < count; ++i) {
        float l = monoBuf[i] * volumeL;
        float r = monoBuf[i] * volumeR;

        if (l > 32767.0f) l = 32767.0f;
        if (l < -32768.0f) l = -32768.0f;
        if (r > 32767.0f) r = 32767.0f;
        if (r < -32768.0f) r = -32768.0f;

        stereoBuf[2 * i] = (int16_t)l;
        stereoBuf[2 * i + 1] = (int16_t)r;
    }

    size_t bytesWritten;
    i2s_write(I2S_NUM_0, stereoBuf, count * 4, &bytesWritten, portMAX_DELAY);
}
