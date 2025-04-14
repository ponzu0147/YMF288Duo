#pragma once

#include <Arduino.h>
#include "S98Parser.hpp"

class S98TimerPlayer {
public:
    S98TimerPlayer();
    void begin(File file);
    void start();
    void stop();
    bool isPlaying() const;

private:
    static void IRAM_ATTR onTimer();
    static hw_timer_t* timer;
    static volatile bool playing;
    static S98Parser parser;
    static volatile uint32_t waitCounter;
    static volatile uint32_t samplesPerTick;

    static void step();  // S98命令を1つ進める
};
