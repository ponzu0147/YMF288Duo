// S98Player.cpp
#include "S98Player.h"
#include "s98.h"
#include <Arduino.h>

File S98Player::s98File;
S98Header S98Player::header;
uint32_t S98Player::waitCounter = 0;
uint8_t S98Player::currentAddress = 0;

// ヘルパー：32bitリトルエンディアン読み取り
uint32_t readLE32(File &file) {
    uint8_t b[4];
    if (file.read(b, 4) != 4) return 0;
    return (uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

bool S98Player::begin(const char* path) {
    s98File = SD.open(path);
    if (!s98File) {
        Serial.println("[S98Player] ファイルが開けません。");
        return false;
    }
    return readHeader();
}

void S98Player::end() {
    if (s98File) s98File.close();
}

bool S98Player::readHeader() {
    if (s98File.readBytes(header.Magic, 3) != 3) return false;
    header.Format = s98File.read();
    header.Timer = readLE32(s98File);
    header.Timer2 = readLE32(s98File);
    header.Compress = readLE32(s98File);
    header.NamePtr = readLE32(s98File);
    header.DataPtr = readLE32(s98File);
    header.LoopPtr = readLE32(s98File);
    header.DeviceCount = readLE32(s98File);
    header.DeviceInfo = readLE32(s98File);

    Serial.printf("[S98Header] Ver: %d  Data: 0x%08lX  Loop: 0x%08lX  Devices: %lu\n",
        header.Format, header.DataPtr, header.LoopPtr, header.DeviceCount);

    // データ領域まで移動
    s98File.seek(header.DataPtr);
    return true;
}

void S98Player::update() {
    if (!s98File || !s98File.available()) return;

    if (waitCounter > 0) {
        --waitCounter;
        return;
    }

    uint8_t cmd = s98File.read();
    handleCommand(cmd);
}

void S98Player::handleCommand(uint8_t cmd) {
    if (cmd <= 0xDF) {
        currentAddress = cmd;
        uint8_t data = s98File.read();
        sendToYMF(currentAddress, data);
    } else if (cmd == 0xFF) {
        waitCounter = 1;
    } else if (cmd == 0xFE) {
        uint8_t lo = s98File.read();
        uint8_t hi = s98File.read();
        waitCounter = lo | (hi << 8);
    } else if (cmd == 0xFD) {
        s98File.seek(header.LoopPtr);
    }
    // 他のコマンドは未対応（将来拡張）
}

void S98Player::sendToYMF(uint8_t addr, uint8_t data) {
    // 仮：YMF288Driver::writeRegister(addr, data);
    Serial.printf("[YMF] Addr: %02X, Data: %02X\n", addr, data);
}