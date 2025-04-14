#include "AudioWavPlayer.hpp"

void AudioWavPlayer::begin() {
    out = new AudioOutputI2S();
    out->begin();
}

void AudioWavPlayer::play(const char* path) {
    if (wav && wav->isRunning()) {
        wav->stop();
    }
    if (file) {
        delete file;
    }

    file = new AudioFileSourceSD(path);
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);
}

void AudioWavPlayer::loop() {
    if (wav && wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
        }
    }
}
