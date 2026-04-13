#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "Def.h"
#include "NFC.h"
#include "Net.h"

Adafruit_PN532 nfc(IRQ_PIN, RST_PIN);

TaskHandle_t nfcTaskHandle = NULL;

bool hasScanned = false;

// ISR
void IRAM_ATTR onCardDetected() {
    BaseType_t taskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(nfcTaskHandle, &taskWoken);
    portYIELD_FROM_ISR(taskWoken);
}

void printUID(uint8_t* uid, uint8_t uidLength) {
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] < 0x10) Serial.print("0"); // Leading zero padding
        Serial.print(uid[i], HEX);
        if (i < uidLength - 1) Serial.print(":");
    }
    Serial.println();
}

void nfcTask(void* param) {
    // Initial IRQ
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    while(1) {
        // Wait for ISR callback to wake the task
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (!hasScanned)
            hasScanned = true;
        else
            continue;

        uint8_t uid[7], uidLength;

        if (nfc.readDetectedPassiveTargetID(uid, &uidLength)) {
            printUID(uid, uidLength);
            if (publishUID(uid, uidLength))
                hasScanned = false;
        }

        delay(SCAN_COOLDOWN);
        nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
        // startPassiveTargetIDDetection spikes the IRQ pin.
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
        Serial.println("Ready. Listening for card...");
    }
}

int nfcInit() {
    // Begin I2C protocol
    Wire.begin(SDA_PIN, SCL_PIN);
    nfc.begin();

    uint32_t version = nfc.getFirmwareVersion();
    if (!version) {
        Serial.println("PN532 Device not found.");
        return 0;
    }

    Serial.println("Found PN532");
    Serial.print("Version: ");
    Serial.print((version >> 16) & 0xFF);
    Serial.print(".");
    Serial.println((version >> 8) & 0xFF);

    nfc.SAMConfig();

    xTaskCreatePinnedToCore(
        nfcTask,        // function to call
        "nfcTask",      // name
        4096,           // stack size
        NULL,           // parameters
        1,              // priority
        &nfcTaskHandle, // handler
        0               // core to work on
    );

    // Attach IRQ pin with callback function
    pinMode(IRQ_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IRQ_PIN), onCardDetected, FALLING);

    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);

    return 1;
}