//Liquid Crystal Display (LCD1602) with I2C backpack example for ESP32-C3
//git@github.com:conspiratus/workshop_l.git

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//pio pkg install --library "marcoschwartz/LiquidCrystal_I2C"


void showSmallHeart();
void showBigHeart();

LiquidCrystal_I2C lcd(0x27, 16, 2);
//LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Small heart — left half
byte smallHeartLeft[8] = {
  B00000,
  B00100,
  B01110,
  B01111,
  B01111,
  B00111,
  B00011,
  B00001
};

// Small heart — right half
byte smallHeartRight[8] = {
  B00000,
  B00100,
  B01110,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};

// Big heart — left half
byte bigHeartLeft[8] = {
  B01100,
  B11110,
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001
};

// Big heart — right half
byte bigHeartRight[8] = {
  B00110,
  B01111,
  B11111,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
};

void setup() {
  // ESP32-C3: SDA = GPIO 8, SCL = GPIO 9
  Wire.begin(8, 9);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("HELLO");

  lcd.setCursor(0, 1);
  lcd.print("ROBERT");

  delay(3000);
}

void loop() {
  showSmallHeart();
  delay(350);

  showBigHeart();
  delay(180);

  showSmallHeart();
  delay(350);

  showBigHeart();
  delay(180);

  delay(700);
}

void showSmallHeart() {
  lcd.createChar(0, smallHeartLeft);
  lcd.createChar(1, smallHeartRight);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.write(byte(1));
}

void showBigHeart() {
  lcd.createChar(0, bigHeartLeft);
  lcd.createChar(1, bigHeartRight);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.write(byte(1));
}
