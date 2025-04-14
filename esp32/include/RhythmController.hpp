#pragma once

#include <Arduino.h>

namespace RhythmController {

// 初期化（自動ハード検知含む）
void begin();

// リズムレジスタ命令を受け取り、適切に振り分けて処理
void handle(uint8_t addr, uint8_t data);

// ハードウェア（YMF288）が接続されているか確認する
bool isHardwareConnected();

}
