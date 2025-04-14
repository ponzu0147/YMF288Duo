#include "S98TimerPlayer.hpp"
#include "AdpcmPlayer.hpp"
#include "RhythmController.hpp"

hw_timer_t* S98TimerPlayer::timer = nullptr;
volatile bool S98TimerPlayer::playing = false;
S98Parser S98TimerPlayer::parser;
volatile uint32_t S98TimerPlayer::waitCounter = 0;
volatile uint32_t S98TimerPlayer::samplesPerTick = 735;  // デフォルト60Hz相当

S98TimerPlayer::S98TimerPlayer() {}

void S98TimerPlayer::begin(File file) {
    parser.open(file);

    // 1tickあたりのサンプル数を計算（Wait命令 = tick単位）
    auto header = parser.getHeader();
    if (header.timerInfoNumerator && header.timerInfoDenominator) {
        samplesPerTick = header.timerInfoDenominator / header.timerInfoNumerator;
    }

    parser.onWriteCommand = [](uint8_t chip, uint8_t addr, uint8_t data) {
        if (chip == 0x00 && addr == 0x68) {
            AdpcmPlayer::feed(data);
        } else if (chip == 0x00 && addr >= 0x10 && addr <= 0x1D) {
            RhythmController::handle(addr, data);
        }
    };

    parser.onWaitCommand = [](uint32_t waitTicks) {
        waitCounter += waitTicks;  // tick単位で蓄積（割り込み側で処理）
    };

    parser.onLoop = []() {
        parser.rewind();
    };

    parser.onEnd = []() {
        parser.rewind();
    };

    parser.rewind();
}

void S98TimerPlayer::start() {
    if (!timer) {
        timer = timerBegin(0, 80, true); // 1us単位タイマー（80分周）
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmWrite(timer, samplesPerTick * 1000 / 55, true);  // tick周期に応じたタイミング
    }
    timerAlarmEnable(timer);
    playing = true;
}

void S98TimerPlayer::stop() {
    if (timer) {
        timerAlarmDisable(timer);
    }
    playing = false;
}

bool S98TimerPlayer::isPlaying() const {
    return playing;
}

void IRAM_ATTR S98TimerPlayer::onTimer() {
    if (playing && waitCounter == 0) {
        step();
    } else if (waitCounter > 0) {
        waitCounter--;
    }
}

void S98TimerPlayer::step() {
    parser.parseNextCommand();  // 1命令だけ処理（WAIT命令なら waitCounter++ される）
}
