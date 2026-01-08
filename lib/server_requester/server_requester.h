#pragma once

#include <stdint.h>
#include <stddef.h>

void setupWiFi(char ssid[], char pass[]);
bool getCardData(uint32_t cardid, char* outBuffer, size_t bufferSize);
