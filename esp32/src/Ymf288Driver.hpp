#pragma once

#include <Arduino.h>

namespace Ymf288Driver {

// 初期化処理（ピン設定など）
void begin();

// レジスタ書き込み（アドレスとデータを指定）
void writeRegister(uint8_t addr, uint8_t data);

}
