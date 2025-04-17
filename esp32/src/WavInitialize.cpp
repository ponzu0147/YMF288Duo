#include "WavInitialize.hpp"
#include <SD.h>
#include "WavPlayer.hpp"

const char* wavNames[6] = {
    "/WAV/2608_BD.wav",
    "/WAV/2608_SD.wav",
    "/WAV/2608_HH.wav",
    "/WAV/2608_TOM.wav",
    "/WAV/2608_RIM.wav",
    "/WAV/2608_TOP.wav"
};

uint8_t* wavData[6] = {nullptr};
size_t wavSize[6] = {0};

bool loadWavToMemory(const char* filename, uint8_t** data, size_t* size) {
    Serial.printf("🔍 Loading: %s\n", filename);
    File file = SD.open(filename);
    if (!file || file.isDirectory()) {
        Serial.println("❌ File not found or is a directory.");
        return false;
    }

    *size = file.size();
    Serial.printf("📦 Size: %u bytes\n", *size);
    *data = (uint8_t*)malloc(*size);  // ← malloc() に変更
    if (!*data) {
        Serial.println("❌ malloc failed");
        file.close();
        return false;
    }

    file.read(*data, *size);
    file.close();
    return true;
}

bool initializeWavFiles() {
    for (int i = 0; i < 6; ++i) {
        if (!loadWavToMemory(wavNames[i], &wavData[i], &wavSize[i])) {
            Serial.printf("Failed to load: %s\n", wavNames[i]);
            return false;
        }
    }
    WavPlayer::setWavBuffers(wavData, wavSize);
    Serial.println("✅ All WAV files loaded into SRAM.");
    return true;
}

void listWavFiles(const char* dirPath) {
    File dir = SD.open(dirPath);
    if (!dir || !dir.isDirectory()) {
        Serial.println("❌ 指定されたディレクトリが存在しません。");
        return;
    }

    Serial.println("📂 /WAV フォルダの内容：");
    File entry;
    while ((entry = dir.openNextFile())) {
        if (!entry.isDirectory()) {
            String filename = entry.name();
            if (filename.endsWith(".wav") || filename.endsWith(".WAV")) {
                Serial.print("🔊 ");
                Serial.println(filename);
            }
        }
        entry.close();
    }
}