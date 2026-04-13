#ifndef NET_H
#define NET_H

#include "secrets.h"

void initMQTT();
void processAndKeepAlive();
int publishUID(uint8_t* uid, uint8_t uidLength);

#define PAYLOAD_SIZE 32

#endif