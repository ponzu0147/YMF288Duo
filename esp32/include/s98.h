#ifndef S98_H
#define S98_H

#define MAXDEVICE    64
#define MAXCHIPTYPE  16

#include <stdint.h>

// S98ファイルヘッダ構造体
struct S98Header {
  char Magic[3];        // "S98"
  char Format;          // バージョン番号（0x30 = ver3）
  uint32_t Timer;       // タイマ情報
  uint32_t Timer2;      // サブタイマ（未使用）
  uint32_t Compress;    // 圧縮（0 = 無圧縮）
  uint32_t NamePtr;     // タグ領域へのポインタ
  uint32_t DataPtr;     // データ領域へのポインタ
  uint32_t LoopPtr;     // ループ開始位置へのポインタ
  uint32_t DeviceCount; // デバイス数
  uint32_t DeviceInfo;  // デバイス情報の先頭ポインタ（S98 v3以降）
};

// ヘッダオフセット定義
enum {
  HEADER_SIZE     = 0x20,
  HEADER_SIZE_MAX = HEADER_SIZE + 0x10 * MAXDEVICE,
  S98_VERSION     = 0x0003,
  TIMER_INFO      = 0x0004,
  TIMER_INFO2     = 0x0008,
  TAG_OFFSET      = 0x0010,
  DATA_OFFSET     = 0x0014,
  LOOP_OFFSET     = 0x0018,
  DEVICE_COUNT    = 0x001C,
};

// デバイス情報オフセット定義
enum {
  TYPE  = 0x00,
  CLOCK = 0x04,
  PAN   = 0x08,
};

// サポートされるチップ種別
enum ChipType {
  NONE = 0,
  YM2149,
  YM2203,
  YM2612,
  YM2608,
  YM2151,
  YM2413,
  YM3526,
  YM3812,
  YMF262,
  RESERVED_10,
  RESERVED_11,
  RESERVED_12,
  RESERVED_13,
  RESERVED_14,
  AY_3_8910 = 15,
  SN76489,
};

#endif // S98_H