// LCD1602 I2C example for ESP32-C3
// Displays text and a custom lightning symbol

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Install:
// pio pkg install --library "marcoschwartz/LiquidCrystal_I2C"

LiquidCrystal_I2C* lcd = nullptr;
uint8_t detectedAddress = 0;
unsigned long lastStatusAt = 0;

// Custom lightning symbol (5x8 pixels)
// byte lightning[8] = {
//   0b10001,
//   0b10001,
//   0b00000,
//   0b11111,
//   0b10001,
//   0b01110,
//   0b00000,
//   0b00000
// };

// byte lightning[8] = {
//   0b00100,
//   0b00100,
//   0b01110,
//   0b01110,
//   0b11111,
//   0b11111,
//   0b00100,
//   0b00100
// };

// byte dog[8] = {
//   0b00000,
//   0b11011,
//   0b10001,
//   0b01010,
//   0b00000,
//   0b11111,
//   0b01110,
//   0b00000
// };

// byte right_eye[8] = {
//   0b11111,
//   0b10001,
//   0b10001,
//   0b10001,
//   0b00011,
//   0b00111,
//   0b10001,
//   0b11111
// };

// byte left_eye[8] = {
//   0b11111,
//   0b10001,
//   0b10001,
//   0b10001,
//   0b11000,
//   0b11100,
//   0b10001,
//   0b11111
// };

// byte right_lip[8] = {
//   0b00000,
//   0b00000,
//   0b00000,
//   0b10000,
//   0b01000,
//   0b00100,
//   0b00011,
//   0b00000
// };

// byte left_lip[8] = {
//   0b00000,
//   0b00000,
//   0b00000,
//   0b00001,
//   0b00010,
//   0b00100,
//   0b11000,
//   0b00000
// };


byte rachet_1[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b11101,
  0b01111,
  0b11101,
  0b00001,
  0b00000
};

byte rachet_2[8] = {
  0b10001,
  0b11011,
  0b10010,
  0b00100,
  0b11000,
  0b11011,
  0b10001,
  0b00001
};


uint8_t findI2cDevice() {
  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.printf("I2C device found at 0x%02X\n", address);
      return address;
    }
  }

  return 0;
}

void setup() {
  Serial.begin(115200);
  const unsigned long serialWaitStartedAt = millis();
  while (!Serial && (millis() - serialWaitStartedAt) < 3000) {
    delay(10);
  }
  Serial.println();
  Serial.println("LCD1602 workshop started");

  // ESP32-C3
  // SDA -> GPIO3
  // SCL -> GPIO4
  Wire.begin(3, 4);

  detectedAddress = findI2cDevice();
  if (detectedAddress == 0) {
    Serial.println("No I2C devices found. Check SDA, SCL, power and GND.");
    return;
  }

  lcd = new LiquidCrystal_I2C(detectedAddress, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();

  // Store lightning symbol in slot 0
  lcd->createChar(0, rachet_1);
  lcd->createChar(1, rachet_2);

  // First row
  lcd->setCursor(0, 0);
  //lcd->print("Hello Pavel!");
  lcd->print("Hello Robert!");

  // Second row
  lcd->setCursor(14, 0);
  lcd->write(byte(0));
  lcd->setCursor(15, 0);
  lcd->write(byte(1));

}

void loop() {
  const unsigned long now = millis();
  if ((now - lastStatusAt) >= 1000) {
    lastStatusAt = now;
    if (detectedAddress != 0) {
      Serial.printf("Alive: LCD at 0x%02X\n", detectedAddress);
    } else {
      Serial.println("Alive: no I2C device detected");
    }
  }

  delay(10);
}
