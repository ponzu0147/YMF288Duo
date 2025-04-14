#pragma once

#include <stdint.h>
#include <vector>
#include <functional>
#include <FS.h>  // SPIFFS/SDなどのファイルシステム

struct S98Header {
    uint8_t formatVersion;
    uint32_t timerInfoNumerator;
    uint32_t timerInfoDenominator;
    uint32_t compressing;
    uint32_t tagOffset;
    uint32_t dataOffset;
    uint32_t loopOffset;
    uint32_t deviceCount;
    uint32_t deviceInfos[3];  // 最大3デバイス想定
};

enum S98CommandType {
    S98_CMD_DATA,
    S98_CMD_WAIT_SHORT,
    S98_CMD_WAIT_LONG,
    S98_CMD_LOOP,
    S98_CMD_END
};

class S98Parser {
public:
    S98Parser();
    bool open(File file);
    bool parseNextCommand();
    void rewind();
    bool isLoopPoint() const;

    // コマンド処理時のコールバック
    std::function<void(uint8_t chip, uint8_t addr, uint8_t data)> onWriteCommand;
    std::function<void(uint32_t waitSamples)> onWaitCommand;
    std::function<void()> onLoop;
    std::function<void()> onEnd;

private:
    File s98File;
    S98Header header;
    uint32_t currentPos;
    uint32_t loopPos;
    bool loopDetected = false;

    bool readHeader();
};
