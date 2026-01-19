#include <Arduino.h>
#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 5, d5 = 6, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

void setupLcd() {
    lcd.begin(16,2);               // initialize the lcd 
    lcd.home();                   
    lcd.print("Hello, ARDUINO ");  
    lcd.createChar(0, bell);
    lcd.createChar(1, note);
    lcd.createChar(2, clock);
    lcd.createChar(3, heart);
    lcd.createChar(4, duck);
    lcd.createChar(5, check);
    lcd.createChar(6, cross);
    lcd.createChar(7, retarrow);
    delay(1000);
}  

// Max 32 characters per string!
void displayText(const char str[]) {
    lcd.home();
    lcd.clear();
    size_t len = strlen(str);
    
    if (len > 16) {
        char firstLine[17]; 
        strncpy(firstLine, str, 16); 
        firstLine[16] = '\0';
        lcd.print(firstLine);

        char secondLine[17];
        size_t lenSecondLine = min(len - 16, (size_t)16);
        strncpy(secondLine, &str[16], lenSecondLine); 
        secondLine[lenSecondLine] = '\0';
        lcd.setCursor(0, 1);
        lcd.print(secondLine);
    } else {
        lcd.print(str);
    }
    delay(1000);
}