#pragma once

// ===== クロック =====
#define PIN_CLK_MCLK     0    // マスタークロック（外部水晶EXO3 15.9744MHzなど）

// ===== DAC I2S出力（PCM5102A等） =====
// config.h
#ifndef PIN_I2S_BCK
#define PIN_I2S_BCK 6
#endif

#define PIN_I2S_LRCK     7
#define PIN_I2S_DATA     8

// ===== YMF288制御ピン（例） =====
#define PIN_YMF_CS       4
#define PIN_YMF_WR       2
#define PIN_YMF_A0       15
#define PIN_YMF_A1       16
#define PIN_YMF_D0       32
#define PIN_YMF_D1       33
#define PIN_YMF_D2       27
#define PIN_YMF_D3       14
#define PIN_YMF_D4       12
#define PIN_YMF_D5       13
#define PIN_YMF_D6       23
#define PIN_YMF_D7       19

// ===== SDカード（SPI接続） =====
#define PIN_SD_CS        10
#define PIN_SD_MOSI      11
#define PIN_SD_MISO      13
#define PIN_SD_SCK       12

// ===== スピーカーアンプ電源制御（任意） =====
// #define PIN_AMP_EN    xx

// ===== デバッグ用ボタン入力（3つ） =====
#define PIN_BTN_NEXT     34  // 曲送りボタン
#define PIN_BTN_PREV     35  // 曲戻りボタン
#define PIN_BTN_PLAY     36  // 再生／一時停止ボタン

// 必要に応じて内部プルアップを使ってINPUT_PULLUPで利用
#define PIN_HW_DETECT 39  // 例: GPIO39（ADC1_CH3）

