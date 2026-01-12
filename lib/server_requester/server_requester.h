#pragma once

#include <stdint.h>
#include <stddef.h>

typedef void (*DisplayCallback)(char* message);
void setDisplayCallback(DisplayCallback callback);

bool setupWiFi(char ssid[], char pass[]);
bool getCardData(uint32_t cardid, char* outBuffer, size_t bufferSize);
