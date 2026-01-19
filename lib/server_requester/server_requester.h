#pragma once

#include <stdint.h>
#include <stddef.h>

typedef void (*DisplayCallback)(const char*);
void setDisplayCallback(DisplayCallback callback);

bool setupWiFi(char ssid[], const char pass[]);
bool getCardData(uint32_t cardid, char* outBuffer, size_t bufferSize);
