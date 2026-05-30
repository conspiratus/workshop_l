// Touch button + transistor motor switch example for ESP32-C3

#include <Arduino.h>

#define BUTTON_PIN 4
#define MOTOR_PIN 5

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);§
  digitalWrite(MOTOR_PIN, buttonState);

  // another way:
  //digitalWrite(MOTOR_PIN, digitalRead(BUTTON_PIN));
}
