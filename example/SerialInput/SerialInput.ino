#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <I2CTextScroller.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
I2CTextScroller scroller(lcd, 16);

char receivedText[64];

void setup() {
  Serial.begin(9600);
  scroller.init();
}

void loop() {
  if (Serial.available() > 0) {
    size_t len = Serial.readBytesUntil('\n', receivedText, sizeof(receivedText) - 1);
    receivedText[len] = '\0';

    scroller.print(receivedText);
  }

  scroller.update();
}
