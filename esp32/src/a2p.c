#include "a2p.h"

// YM2608/OPNA ADPCMデコードのためのステップサイズテーブル
static const int step_table[16] = {
     0,  1,  2,  4,  8, 16, 32, 64,
     0, -1, -2, -4, -8, -16, -32, -64
};

static int16_t adpcm_decode_nibble(uint8_t nibble, int16_t* predictor) {
    int step = step_table[nibble & 0x0F];
    *predictor += step;

    // オーバーフロー防止
    if (*predictor > 32767) *predictor = 32767;
    if (*predictor < -32768) *predictor = -32768;

    return *predictor;
}

void a2p_decode_single(uint8_t adpcm_byte, int16_t* pcm_out) {
    static int16_t predictor = 0;

    uint8_t high = (adpcm_byte >> 4) & 0x0F;
    uint8_t low  = adpcm_byte & 0x0F;

    pcm_out[0] = adpcm_decode_nibble(high, &predictor);
    pcm_out[1] = adpcm_decode_nibble(low, &predictor);
}

size_t a2p_decode(const uint8_t* adpcm_data, size_t length, int16_t* pcm_out) {
    static int16_t predictor = 0;
    size_t pcm_index = 0;

    for (size_t i = 0; i < length; ++i) {
        uint8_t byte = adpcm_data[i];
        uint8_t high = (byte >> 4) & 0x0F;
        uint8_t low  = byte & 0x0F;

        pcm_out[pcm_index++] = adpcm_decode_nibble(high, &predictor);
        pcm_out[pcm_index++] = adpcm_decode_nibble(low, &predictor);
    }

    return pcm_index;
}
