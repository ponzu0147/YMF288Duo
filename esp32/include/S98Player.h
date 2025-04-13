// S98Player.h
#ifndef S98PLAYER_H
#define S98PLAYER_H

#include <SD.h>
#include <stdint.h>

class S98Player {
public:
    static bool begin(const char* path);  // S98ファイルの初期化
    static void update();                 // コマンド処理の1ステップ実行
    static void end();                    // ファイルクローズなどの終了処理

private:
    struct S98Header {
        char signature[3];
        uint8_t version;
        uint32_t timerInfo[3];
        uint32_t compressType;
        uint32_t tagOffset;
        uint32_t dataOffset;
        uint32_t loopOffset;
        uint32_t deviceCount;
    };

    static File s98File;
    static S98Header header;
    static uint32_t waitCounter;
    static uint8_t currentAddress;

    static bool readHeader();                 // ヘッダー解析処理
    static void handleCommand(uint8_t cmd);   // コマンド解析と実行
    static void sendToYMF(uint8_t addr, uint8_t data); // YMF288レジスタ送信
};

#endif // S98PLAYER_H
