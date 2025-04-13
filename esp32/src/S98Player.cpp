// S98Player.cpp
#include "S98Player.h"
#include <Arduino.h>

File S98Player::s98File;
S98Player::S98Header S98Player::header;
uint32_t S98Player::waitCounter = 0;
uint8_t S98Player::currentAddress = 0;

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
    if (s98File.readBytes(header.signature, 3) != 3) return false;
    header.version = s98File.read();
    for (int i = 0; i < 3; ++i) header.timerInfo[i] = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);
    header.compressType = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);
    header.tagOffset = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);
    header.dataOffset = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);
    header.loopOffset = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);
    header.deviceCount = s98File.read() | (s98File.read() << 8) | (s98File.read() << 16) | (s98File.read() << 24);

    s98File.seek(header.dataOffset);
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
        s98File.seek(header.loopOffset);
    }
    // 他のコマンドは未対応（将来拡張）
}

void S98Player::sendToYMF(uint8_t addr, uint8_t data) {
    // 仮：YMF288Driver::writeRegister(addr, data);
    Serial.printf("[YMF] Addr: %02X, Data: %02X\n", addr, data);
}