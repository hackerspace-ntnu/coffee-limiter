#ifndef DEF_H
#define DEF_H

#include <Adafruit_NeoPixel.h>

// LED
#define LED_PIN   48
#define NUM_LEDS  1

extern Adafruit_NeoPixel led;

#define BOOTING     led.Color(0, 255, 0)
#define ERROR       led.Color(255, 0, 0)
#define MOUSE       led.Color(0, 0, 255)
#define SCANNING    led.Color(0, 255, 255)

// GPIO
#define BOOT_BUTTON 0

// States
enum ESP32_STATES {
    CARD_SCANNING, COFFEE_SELECT
};

extern ESP32_STATES MODE;

#endif