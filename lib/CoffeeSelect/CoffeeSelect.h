#ifndef COFFEE_SELECT_H
#define COFFEE_SELECT_H

#include <USBHIDMouse.h>

extern USBHIDMouse mouse;

void handleCoffeeRequest(char* message);
void handleCoordinatesRequest(int x, int y, bool mouseClick, int msDelay);

#endif