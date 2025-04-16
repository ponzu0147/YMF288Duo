#include "WavPlayer.hpp"
#include <Arduino.h>
#include <SD.h>
#include <driver/i2s.h>
#include <math.h>  // fabs

#define I2S_BCLK 6
#define I2S_LRC  7
#define I2S_DOUT 8

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
}

File wavFile;
bool playing = false;
float volumeL = 1.0f;
float volumeR = 1.0f;

void WavPlayer::begin() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

bool WavPlayer::playStereo(const char* filepath, float volL_in, float volR_in) {
    stop();

    wavFile = SD.open(filepath);
    if (!wavFile) {
        Serial.printf("❌ WAVファイルオープン失敗: %s\n", filepath);
        return false;
    }

    for (int i = 0; i < 44; ++i) wavFile.read();

    // ボリュームの値が正常か確認（NaNや極端な値を防ぐ）
    volumeL = (volL_in >= 0.0f && volL_in <= 1.0f) ? volL_in : 1.0f;
    volumeR = (volR_in >= 0.0f && volR_in <= 1.0f) ? volR_in : 1.0f;

    playing = true;
    Serial.printf("▶️ 再生開始: %s\n", filepath);
    return true;
}

void WavPlayer::stop() {
    if (playing) {
        wavFile.close();
        playing = false;
        Serial.println("⏹️ 再生停止");

        // ビープ音対策：DMA完全初期化
        i2s_zero_dma_buffer(I2S_NUM_0);
    }
}

void WavPlayer::loop() {
    if (!playing) return;

    uint8_t buffer[512];
    size_t bytesRead = wavFile.read(buffer, sizeof(buffer));
    if (bytesRead == 0) {
        // 再生終了時：ゼロバッファでDACを静音に戻す
        int16_t zero[1024] = {0};
        size_t dummy;
        for (int i = 0; i < 4; ++i) {
            i2s_write(I2S_NUM_0, zero, sizeof(zero), &dummy, portMAX_DELAY);
        }
        delay(30);  // DMA排出を待機
        stop();
        Serial.println("🔇 再生完了（EOF）");
        return;
    }

    int16_t* in = reinterpret_cast<int16_t*>(buffer);
    int samplesRead = bytesRead / 2;
    if (samplesRead <= 0) {
        stop();
        return;
    }

    float globalGain = 0.3f;
    float panL = volumeL;
    float panR = volumeR;

    // Cパン時（両チャンネルとも1.0）なら 0.707 補正
    if (fabs(volumeL - 1.0f) < 0.01f && fabs(volumeR - 1.0f) < 0.01f) {
        panL = panR = 0.707f;
    }

    int16_t stereoBuffer[1024];
    size_t maxSamples = sizeof(stereoBuffer) / sizeof(int16_t) / 2;
    samplesRead = min(samplesRead, (int)maxSamples);

    for (int i = 0; i < samplesRead; ++i) {
        int32_t left  = static_cast<int32_t>(in[i] * panL * globalGain);
        int32_t right = static_cast<int32_t>(in[i] * panR * globalGain);

        stereoBuffer[i * 2]     = clamp(left, -32768, 32767);
        stereoBuffer[i * 2 + 1] = clamp(right, -32768, 32767);
    }

    size_t bytesWritten;
    i2s_write(I2S_NUM_0, stereoBuffer, samplesRead * 2 * sizeof(int16_t), &bytesWritten, portMAX_DELAY);

    Serial.printf("🎧 %dサンプル読み込み、📤 %dバイト出力\n", samplesRead, (int)bytesWritten);
}

bool WavPlayer::isPlaying() const {
    return playing;
}
