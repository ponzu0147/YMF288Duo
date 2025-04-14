#pragma once

#include <Arduino.h>

namespace RhythmPlayer {

// 初期化（WAVファイル一覧の確認など）
void begin();

// YM2608のリズムレジスタ書き込み処理（S98経由）
void handle(uint8_t addr, uint8_t data);

}
