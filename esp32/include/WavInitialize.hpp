#pragma once
#include <Arduino.h>

extern const char* wavNames[6];
extern uint8_t* wavData[6];
extern size_t wavSize[6];

bool initializeWavFiles();
void listWavFiles(const char* dirPath = "/WAV");