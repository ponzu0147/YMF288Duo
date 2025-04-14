#include "AdpcmPlayer.hpp"
#include "a2p.h"  // ADPCM→PCM変換ライブラリ

namespace {
    constexpr int I2S_SAMPLE_RATE = 22050;
    constexpr i2s_port_t I2S_PORT = I2S_NUM_0;
    constexpr size_t PCM_BUFFER_SIZE = 1024;
    int16_t pcmBuffer[PCM_BUFFER_SIZE];
    size_t pcmIndex = 0;
}

namespace AdpcmPlayer {

void begin() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = 4,
        .dma_buf_len = 256,
        .use_apll = false
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 22,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
    i2s_set_pin(I2S_PORT, &pin_config);
}

void feed(uint8_t adpcmByte) {
    int16_t pcmOut[2];
    a2p_decode_single(adpcmByte, pcmOut);  // 4bit x 2 サンプルを変換

    // PCM出力バッファに追加
    for (int i = 0; i < 2; ++i) {
        pcmBuffer[pcmIndex++] = pcmOut[i];
        if (pcmIndex >= PCM_BUFFER_SIZE) {
            size_t written;
            i2s_write(I2S_PORT, pcmBuffer, PCM_BUFFER_SIZE * sizeof(int16_t), &written, portMAX_DELAY);
            pcmIndex = 0;
        }
    }
}
}
