#pragma once

// ========================
// YMF288 接続ピン定義（S3用）
// ========================
#define PIN_YMF_WR     6   // 書き込み
#define PIN_YMF_CS     7   // チップセレクト
#define PIN_YMF_A0     15  // アドレス0
#define PIN_YMF_A1     16  // アドレス1

// データバス D0〜D7（重複や制限を避けて割当）
#define PIN_YMF_D0     9
#define PIN_YMF_D1     10
#define PIN_YMF_D2     11
#define PIN_YMF_D3     12
#define PIN_YMF_D4     13
#define PIN_YMF_D5     14
#define PIN_YMF_D6     17
#define PIN_YMF_D7     18

// ========================
// I2S（DAC）出力ピン定義（I2S0）
// ========================
#define PIN_I2S_BCK     4   // Bit Clock
#define PIN_I2S_LRCK    5   // Word Select
#define PIN_I2S_DATA    3   // Serial Data

// ========================
// SDカード SPI接続ピン定義（VSPI or 任意SPI）
// ========================
#define PIN_SD_CS      38
#define PIN_SD_MOSI    35
#define PIN_SD_MISO    36
#define PIN_SD_SCK     37

// ========================
// ハードウェア検出ピン
// ========================
#define PIN_HW_DETECT   8   // /COMピンなどに接続予定

// ========================
// デバッグLEDなど
// ========================
#define PIN_LED         48  // ESP32-S3標準LEDピン
