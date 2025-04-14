#pragma once

#include <Arduino.h>
#include <driver/i2s.h>

namespace AdpcmPlayer {

// 初期化（I2S設定）
void begin();

// ADPCMデータを1バイトずつフィード（S98の0x68命令などから呼ぶ）
void feed(uint8_t adpcmByte);

// 内部でADPCM → PCM変換し、リングバッファに格納
// バックグラウンドでI2S出力処理が走る（DMA）

}
