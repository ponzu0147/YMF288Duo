#pragma once
#include <Arduino.h>
#include <FS.h>

class WavPlayer {
public:
    void begin();
    bool play(const char* filepath);                       // モノラル再生（廃止予定）
    bool playStereo(const char* filepath, float volL, float volR); // ステレオ再生
    void stop();                                           // 強制停止
    void loop();                                           // 再生継続処理
    bool isPlaying() const;

private:
    File wavFile;
    bool playing = false;
    float volumeL = 1.0f;
    float volumeR = 1.0f;
};
