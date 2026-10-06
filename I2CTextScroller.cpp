#include "I2CTextScroller.h"

I2CTextScroller::I2CTextScroller(LiquidCrystal_I2C& lcd, uint8_t lcdCols)
    : _lcd(lcd), _lcdCols(lcdCols), _col(0), _row(0),
      _textLength(0), _scrollingAmount(0), _scrollingTimes(0),
      _scrollDelayMs(1000), _lastReceiveTime(0), _lastScrollTime(0) {
    _textBuffer[0] = '\0';
}

void I2CTextScroller::init() {
    _lcd.init();
    _lcd.backlight();
}

void I2CTextScroller::backlight() {
    _lcd.backlight();
}

void I2CTextScroller::noBacklight() {
    _lcd.noBacklight();
}

void I2CTextScroller::setCursor(uint8_t col, uint8_t row) {
    _col = col;
    _row = row;

    // If text is already set, re-render at new cursor position
    if (_textLength > 0) {
        resetLCDState();
        _lcd.setCursor(_col, _row);
        _lcd.print(_textBuffer);
    }
}

void I2CTextScroller::print(const char* text) {
    if (text == nullptr) return;

    // Copy string into fixed buffer safely
    strncpy(_textBuffer, text, MAX_TEXT_LEN - 1);
    _textBuffer[MAX_TEXT_LEN - 1] = '\0';

    _textLength = strlen(_textBuffer);
    _scrollingAmount = (_textLength > _lcdCols) ? (_textLength - _lcdCols) : 0;

    resetLCDState();
    _lcd.setCursor(_col, _row);
    _lcd.print(_textBuffer);

    _lastReceiveTime = millis();
}

void I2CTextScroller::print(const String& text) {
    print(text.c_str());
}

void I2CTextScroller::clear() {
    _textBuffer[0] = '\0';
    _textLength = 0;
    _scrollingAmount = 0;
    _scrollingTimes = 0;
    resetLCDState();
}

void I2CTextScroller::setScrollDelay(uint16_t delayMs) {
    _scrollDelayMs = delayMs;
}

void I2CTextScroller::update() {
    if (_textLength <= _lcdCols) return;

    unsigned long currentTime = millis();

    // Initial pause before scrolling starts
    if (currentTime - _lastReceiveTime < _scrollDelayMs) return;

    // Step scrolling timer
    if (currentTime - _lastScrollTime >= _scrollDelayMs) {
        if (_scrollingTimes >= _scrollingAmount) {
            resetLCDState();

            _lcd.setCursor(_col, _row);
            _lcd.print(_textBuffer);

            _scrollingTimes = 0;
            _lastScrollTime = currentTime;
            return;
        }

        _lcd.scrollDisplayLeft();
        _lastScrollTime = currentTime;
        _scrollingTimes++;
    }
}

void I2CTextScroller::resetLCDState() {
    _lcd.clear(); // Resets display shift hardware registers
    _lastScrollTime = 0;
}