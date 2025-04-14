#pragma once

// ========================
// YMF288 接続ピン定義（S3用）
// ========================
#define PIN_YMF_WR     6
#define PIN_YMF_CS     7
#define PIN_YMF_A0     15
#define PIN_YMF_A1     16

#define PIN_YMF_D0     9
#define PIN_YMF_D1     10
#define PIN_YMF_D2     11
#define PIN_YMF_D3     12
#define PIN_YMF_D4     13
#define PIN_YMF_D5     14
#define PIN_YMF_D6     17
#define PIN_YMF_D7     18

// ========================
// I2S（DAC）出力ピン定義（ESP32-S3動作実績あり）
// ========================
#define PIN_I2S_BCK     42  // Bit Clock
#define PIN_I2S_LRCK    41  // Word Select
#define PIN_I2S_DATA    2   // Serial Data

// ========================
// SDカード SPI接続ピン定義
// ========================
#define PIN_SD_CS      38
#define PIN_SD_MOSI    35
#define PIN_SD_MISO    36
#define PIN_SD_SCK     37

// ========================
// ハードウェア検出ピン
// ========================
#define PIN_HW_DETECT   8

// ========================
// デバッグLEDなど
// ========================
#define PIN_LED         48
