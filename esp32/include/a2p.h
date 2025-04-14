#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ADPCMバッファをPCMに変換（最大2サンプル）
// input: 4bit ADPCM 1バイト（上下4bitで2サンプル）
// output: 16bit PCM 2サンプル分のバッファ
void a2p_decode_single(uint8_t adpcm_byte, int16_t* pcm_out);

// ADPCMバッファ全体をPCMに変換（バッファ形式）
// input: ADPCMデータ列
// output: PCM出力先バッファ、戻り値はPCMサンプル数
size_t a2p_decode(const uint8_t* adpcm_data, size_t length, int16_t* pcm_out);

#ifdef __cplusplus
}
#endif
