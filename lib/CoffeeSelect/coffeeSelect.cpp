#include <Arduino.h>
#include <USB.h>
#include <map>
#include "CoffeeSelect.h"
#include "Def.h"

void moveToCoffee();
void moveToCoffeeMilk();
void moveToCappuccino();
void moveToChocolateMilk();
void moveToWienerMelange();
void moveToCoffeeChoco();
void moveToLatteMacchiato();
void moveToWarmWater();


std::map<std::string, void(*)()> drinkMap = {
    { "coffee", moveToCoffee },
    { "coffee_milk", moveToCoffeeMilk },
    { "cappuccino", moveToCappuccino },
    { "chocolate_milk", moveToChocolateMilk },
    { "wiener_melange", moveToWienerMelange },
    { "coffee_choco", moveToCoffeeChoco },
    { "latte_macchiato", moveToLatteMacchiato },
    { "warm_water", moveToWarmWater }
};

void moveTo(int x, int y) {
    int xIter = (x / 127) + 1;
    int xRest = x % 127;
    int yIter = (y / 127) + 1;
    int yRest = y % 127;

    for (int i = 0; i < xIter; i++) {
        if (i == xIter - 1) {
            mouse.move(xRest, 0, 0);
        } else {
            mouse.move(127, 0, 0);
        }
    }

    for (int i = 0; i < yIter; i++) {
        if (i == yIter - 1) {
            mouse.move(0, yRest, 0);
        } else {
            mouse.move(0, 127, 0);
        }
    }
}

void moveToAndReset(int x, int y, bool clickAfterMove) {
    mouse.click();
    delay(300);
    moveTo(x, y);
    delay(300);
    if (clickAfterMove)
        mouse.click();
    delay(300);
    moveTo(-x, -y);
}

void handleCoffeeRequest(char* message) {
    auto value = drinkMap.find(message);
    if (value != drinkMap.end()) {
        value->second();
    }
}

void handleCoordinatesRequest(int x, int y, bool mouseClick, int msDelay) {
    moveTo(x, y);

    if (msDelay > 0) {
        mouse.press();
        delay(msDelay);
        mouse.release();
    } else {
        mouse.click();
    }
}

void moveToCoffee() {
    moveToAndReset(-100, -70, true);
}

void moveToCoffeeMilk() {
    moveToAndReset(100, -70, true);
}

void moveToCappuccino() {
    moveToAndReset(-100, -20, true);
}

void moveToChocolateMilk() {
    moveToAndReset(100, -20, true);
}

void moveToWienerMelange() {
    moveToAndReset(-100, 40, true);
}

void moveToCoffeeChoco() {
    moveToAndReset(100, 40, true);
}

void moveToLatteMacchiato() {
    moveToAndReset(-100, 80, true);
}

void moveToWarmWater() {
    moveToAndReset(100, 80, true);
}