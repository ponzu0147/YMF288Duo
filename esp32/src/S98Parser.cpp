#include "S98Parser.hpp"

S98Parser::S98Parser() {}

bool S98Parser::open(File file) {
    s98File = file;
    currentPos = 0;
    loopDetected = false;
    return readHeader();
}

void S98Parser::rewind() {
    if (s98File) {
        s98File.seek(header.dataOffset);
        currentPos = header.dataOffset;
        loopDetected = false;
    }
}

bool S98Parser::isLoopPoint() const {
    return loopDetected;
}

bool S98Parser::readHeader() {
    if (!s98File) return false;
    s98File.seek(0);
    char magic[3];
    s98File.readBytes(magic, 3);
    if (strncmp(magic, "S98", 3) != 0) return false;

    header.formatVersion = s98File.read();
    s98File.read((uint8_t*)&header.timerInfoNumerator, 4);
    s98File.read((uint8_t*)&header.timerInfoDenominator, 4);
    s98File.read((uint8_t*)&header.compressing, 4);
    s98File.read((uint8_t*)&header.tagOffset, 4);
    s98File.read((uint8_t*)&header.dataOffset, 4);
    s98File.read((uint8_t*)&header.loopOffset, 4);
    s98File.read((uint8_t*)&header.deviceCount, 4);
    s98File.read((uint8_t*)&header.deviceInfos, 12);
    return true;
}

bool S98Parser::parseNextCommand() {
    if (!s98File || !s98File.available()) return false;

    uint8_t cmd = s98File.read();
    currentPos++;

    if (cmd == 0xFF) {
        if (onEnd) onEnd();
        return false;
    } else if (cmd == 0xFE) {
        if (onLoop) onLoop();
        loopDetected = true;
        return true;
    } else if (cmd == 0xFD) {
        if (onWaitCommand) onWaitCommand(735);  // 60Hz frame wait
        return true;
    } else if (cmd == 0xFC) {
        if (onWaitCommand) onWaitCommand(882);  // 50Hz frame wait
        return true;
    } else if ((cmd & 0xF8) == 0x00) {
        uint8_t addr = s98File.read();
        uint8_t data = s98File.read();
        if (onWriteCommand) onWriteCommand(cmd, addr, data);
        currentPos += 2;
        return true;
    } else {
        // 未対応 or 拡張コマンド
        return false;
    }
}
