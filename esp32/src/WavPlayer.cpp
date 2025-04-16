#include "WavPlayer.hpp"
#include <Arduino.h>
#include <SD.h>
#include <driver/i2s.h>

// I2Sピン設定（必要に応じて変更）
#define I2S_BCLK 26
#define I2S_LRC  25
#define I2S_DOUT 22

// clamp 関数（C++17未満環境対応）
template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
}

// 内部変数（外部クラス設計に影響しない範囲で保持）
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
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // ← 非推奨対応済
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

    // WAVヘッダー（44バイト）をスキップ
    for (int i = 0; i < 44; ++i) wavFile.read();

    volumeL = volL_in;
    volumeR = volR_in;
    playing = true;

    Serial.printf("▶️ 再生開始: %s\n", filepath);
    return true;
}

void WavPlayer::stop() {
    if (playing) {
        wavFile.close();
        playing = false;
        Serial.println("⏹️ 再生停止");
    }
}

void WavPlayer::loop() {
    if (!playing) return;

    uint8_t buffer[512];
    size_t bytesRead = wavFile.read(buffer, sizeof(buffer));
    if (bytesRead == 0) {
        stop();
        Serial.println("🔇 再生完了（EOF）");
        return;
    }

    int16_t* in = reinterpret_cast<int16_t*>(buffer);
    int samplesRead = bytesRead / 2;

    // 音量調整＋パンニング＋ステレオ化
    float globalGain = 0.3f;
    int16_t stereoBuffer[1024];

    for (int i = 0; i < samplesRead; ++i) {
        int32_t left  = static_cast<int32_t>(in[i] * volumeL * globalGain);
        int32_t right = static_cast<int32_t>(in[i] * volumeR * globalGain);

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
