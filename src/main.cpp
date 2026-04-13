#include <Arduino.h>
#include <USB.h>
#include <USBHIDMouse.h>
#include "Def.h"
#include "NFC.h"
#include "Net.h"
#include "CoffeeSelect.h"

ESP32_STATES MODE = COFFEE_SELECT;

// Mouse
USBHIDMouse mouse;
Adafruit_NeoPixel led(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    led.setPixelColor(0, BOOTING);
    led.show();
    Serial.println("Booting up device");
    Serial.println("Setting device as a Mouse Device");
    mouse.begin();
    USB.begin();

    delay(2000);

    Serial.println("Starting MQTT Connection");
    initMQTT();

    if (MODE = CARD_SCANNING) {
        Serial.println("Starting RFID scanning");
        led.setPixelColor(0, SCANNING);
        led.show();

        if (!nfcInit())
            Serial.println("Please restart the device");
    }
}

void loop() {
    processAndKeepAlive();
}