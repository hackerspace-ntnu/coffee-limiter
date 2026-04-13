#ifndef NFC_H
#define NFC_H

#define SDA_PIN  8
#define SCL_PIN  9
#define IRQ_PIN  4
#define RST_PIN -1

#define SCAN_COOLDOWN 500 // 1 seconds

int nfcInit();

#endif