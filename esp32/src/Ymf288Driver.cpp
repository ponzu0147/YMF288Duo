#include "Ymf288Driver.hpp"
#include "config.h"  // ← S3用 config

void setDataBus(uint8_t data) {
    digitalWrite(PIN_YMF_D0, (data >> 0) & 1);
    digitalWrite(PIN_YMF_D1, (data >> 1) & 1);
    digitalWrite(PIN_YMF_D2, (data >> 2) & 1);
    digitalWrite(PIN_YMF_D3, (data >> 3) & 1);
    digitalWrite(PIN_YMF_D4, (data >> 4) & 1);
    digitalWrite(PIN_YMF_D5, (data >> 5) & 1);
    digitalWrite(PIN_YMF_D6, (data >> 6) & 1);
    digitalWrite(PIN_YMF_D7, (data >> 7) & 1);
}

void Ymf288Driver::begin() {
    pinMode(PIN_YMF_WR, OUTPUT);
    pinMode(PIN_YMF_CS, OUTPUT);
    pinMode(PIN_YMF_A0, OUTPUT);
    pinMode(PIN_YMF_A1, OUTPUT);

    pinMode(PIN_YMF_D0, OUTPUT);
    pinMode(PIN_YMF_D1, OUTPUT);
    pinMode(PIN_YMF_D2, OUTPUT);
    pinMode(PIN_YMF_D3, OUTPUT);
    pinMode(PIN_YMF_D4, OUTPUT);
    pinMode(PIN_YMF_D5, OUTPUT);
    pinMode(PIN_YMF_D6, OUTPUT);
    pinMode(PIN_YMF_D7, OUTPUT);

    digitalWrite(PIN_YMF_CS, HIGH);
    digitalWrite(PIN_YMF_WR, HIGH);
}

void Ymf288Driver::writeRegister(uint8_t addr, uint8_t data) {
    digitalWrite(PIN_YMF_CS, LOW);
    digitalWrite(PIN_YMF_A0, LOW);
    digitalWrite(PIN_YMF_A1, LOW);
    setDataBus(addr);
    digitalWrite(PIN_YMF_WR, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_YMF_WR, HIGH);

    digitalWrite(PIN_YMF_A0, HIGH);
    setDataBus(data);
    digitalWrite(PIN_YMF_WR, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_YMF_WR, HIGH);
    digitalWrite(PIN_YMF_CS, HIGH);
}
