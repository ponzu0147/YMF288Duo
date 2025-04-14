#pragma once

#include <stdint.h>
#include <functional>
#include <FS.h>  // ArduinoでSD使用時に必要

struct S98Header {
    uint8_t formatVersion;
    uint32_t timerInfoNumerator;
    uint32_t timerInfoDenominator;
    uint32_t compressing;
    uint32_t tagOffset;
    uint32_t dataOffset;
    uint32_t loopOffset;
    uint32_t deviceCount;
    uint32_t deviceInfos[3];  // 最大3デバイス対応
};

class S98Parser {
public:
    S98Parser();
    bool open(File file);
    void rewind();
    bool parseNextCommand();
    bool isLoopPoint() const;
    S98Header getHeader() const;

    std::function<void(uint8_t chip, uint8_t addr, uint8_t data)> onWriteCommand;
    std::function<void(uint32_t waitTicks)> onWaitCommand;
    std::function<void()> onLoop;
    std::function<void()> onEnd;

private:
    File s98File;
    S98Header header;
    uint32_t currentPos;
    bool loopDetected = false;

    bool readHeader();
};
