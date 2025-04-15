
#include "WavPlayer.hpp"
#include <AudioTools.h>
#include <Arduino.h>
#include <SD.h>
#include <algorithm>  // std::clamp に必要

I2SStream i2s;
WAVDecoder decoder;
StreamCopy copier;

void WavPlayer::begin() {
    auto config = i2s.defaultConfig();
    config.bits_per_sample = 16;
    config.sample_rate = 44100;
    config.channels = 2;
    config.pin_bck = 26;
    config.pin_ws = 25;
    config.pin_data = 22;
    i2s.begin(config);
}

bool WavPlayer::playStereo(const char* filepath, float volL, float volR) {
    stop();
    wavFile = SD.open(filepath);
    if (!wavFile) {
        Serial.printf("❌ WAVファイルオープン失敗: %s\n", filepath);
        return false;
    }

    if (!decoder.begin(wavFile)) {
        Serial.println("❌ WAVデコーダ初期化失敗");
        wavFile.close();
        return false;
    }

    playing = true;
    volumeL = volL;
    volumeR = volR;
    return true;
}

void WavPlayer::stop() {
    if (playing) {
        decoder.end();
        wavFile.close();
        playing = false;
        Serial.println("⏹️ 再生停止");
    }
}

void WavPlayer::loop() {
    if (!playing) return;

    int16_t samples[256];
    int samplesRead = decoder.read(samples, 256);

    if (samplesRead == 0) {
        stop();
        Serial.println("🔇 再生完了（EOF）");
        return;
    }

    // グローバルゲインで音量抑制
    float globalGain = 0.3f;

    // ステレオバッファへ変換（音量適用＆クリッピング）
    int16_t stereoBuffer[512];
    for (int i = 0; i < samplesRead; ++i) {
        int32_t left  = static_cast<int32_t>(samples[i] * volumeL * globalGain);
        int32_t right = static_cast<int32_t>(samples[i] * volumeR * globalGain);

        stereoBuffer[i * 2]     = std::clamp(left, -32768, 32767);
        stereoBuffer[i * 2 + 1] = std::clamp(right, -32768, 32767);
    }

    // I2Sへ出力
    size_t bytesWritten = i2s.write(stereoBuffer, samplesRead * 2 * sizeof(int16_t));

    // ログ出力
    Serial.printf("🎧 %dサンプル読み込み、📤 %dバイト出力
", samplesRead, (int)bytesWritten);
}

bool WavPlayer::isPlaying() const {
    return playing;
}
