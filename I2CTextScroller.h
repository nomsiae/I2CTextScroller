#ifndef I2C_TEXT_SCROLLER_H
#define I2C_TEXT_SCROLLER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class I2CTextScroller {
public:
    // Buffer size constant for maximum string length supported
    static const uint8_t MAX_TEXT_LEN = 64;

    I2CTextScroller(LiquidCrystal_I2C& lcd, uint8_t lcdCols = 16);

    void init();
    void backlight();
    void noBacklight();
    void setCursor(uint8_t col, uint8_t row);
    void print(const char* text);
    void print(const String& text);
    void clear();
    void setScrollDelay(uint16_t delayMs);
    void update();

private:
    LiquidCrystal_I2C& _lcd;
    uint8_t _lcdCols;
    
    char _textBuffer[MAX_TEXT_LEN];
    uint8_t _textLength;
    
    uint8_t _col;
    uint8_t _row;
    
    uint8_t _scrollingAmount;
    uint8_t _scrollingTimes;
    
    uint16_t _scrollDelayMs;
    unsigned long _lastReceiveTime;
    unsigned long _lastScrollTime;

    void resetLCDState();
};

#endif