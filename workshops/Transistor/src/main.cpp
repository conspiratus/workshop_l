// Touch button + transistor motor switch example for ESP32-C3

#include <Arduino.h>
#include <esp_system.h>

#define BUTTON_PIN 3
#define MOTOR_PIN 1

namespace {
constexpr bool kButtonActiveHigh = true;
constexpr unsigned long kDebounceMs = 30;
constexpr unsigned long kMinMotorSwitchMs = 250;
constexpr unsigned long kStatusPrintMs = 1000;

bool rawButtonPressed() {
  const int rawLevel = digitalRead(BUTTON_PIN);
  return kButtonActiveHigh ? (rawLevel == HIGH) : (rawLevel == LOW);
}

void applyMotorState(bool enabled) {
  digitalWrite(MOTOR_PIN, enabled ? HIGH : LOW);
}
}  // namespace

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(MOTOR_PIN, OUTPUT);
  applyMotorState(false);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Transistor workshop started");
  Serial.printf("Reset reason: %d\n", static_cast<int>(esp_reset_reason()));
}

void loop() {
  static bool lastRawState = rawButtonPressed();
  static bool stableState = lastRawState;
  static bool motorState = false;
  static unsigned long lastChangeAt = millis();
  static unsigned long lastMotorSwitchAt = 0;
  static unsigned long lastStatusPrintAt = 0;

  const bool currentRawState = rawButtonPressed();
  const unsigned long now = millis();

  if (currentRawState != lastRawState) {
    lastRawState = currentRawState;
    lastChangeAt = now;
  }

  if ((now - lastChangeAt) >= kDebounceMs && stableState != currentRawState) {
    stableState = currentRawState;
    Serial.printf("Button changed: raw=%d stable=%d motor=%d\n", currentRawState, stableState, motorState);
  }

  if (motorState != stableState && (now - lastMotorSwitchAt) >= kMinMotorSwitchMs) {
    motorState = stableState;
    lastMotorSwitchAt = now;
    applyMotorState(motorState);
    Serial.printf("Button=%d Motor=%d\n", stableState, motorState);
  }

  if ((now - lastStatusPrintAt) >= kStatusPrintMs) {
    lastStatusPrintAt = now;
    Serial.printf("Status: raw=%d stable=%d motor=%d\n", currentRawState, stableState, motorState);
  }

  delay(5);
}
