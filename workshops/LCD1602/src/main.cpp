// LCD1602 I2C example for ESP32-C3
// Displays text and a custom lightning symbol

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Install:
// pio pkg install --library "marcoschwartz/LiquidCrystal_I2C"

// LCD I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);
// LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Custom lightning symbol (5x8 pixels)
byte lightning[8] = {
  B00100,
  B01100,
  B01000,
  B11110,
  B00100,
  B01100,
  B01000,
  B00000
};

void setup() {

  // ESP32-C3
  // SDA -> GPIO8
  // SCL -> GPIO9
  Wire.begin(8, 9);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Store lightning symbol in slot 0
  lcd.createChar(0, lightning);

  // First row
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");

  // Second row
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
}

void loop() {
}