// Touch button controlled LED animation modes for ESP32-C3.

#include <Arduino.h>

#define BUTTON_PIN 0

namespace {
constexpr uint8_t kLedPins[] = {
  5,  // Green
  6,  // Yellow
  7,  // Red
  8,  // White
  9   // Blue
};

constexpr size_t kLedCount = sizeof(kLedPins) / sizeof(kLedPins[0]);
constexpr bool kButtonActiveHigh = true;
constexpr uint8_t kBrightnessPercent = 10;
constexpr unsigned long kDebounceMs = 35;
constexpr unsigned long kLongPressMs = 3000;
constexpr unsigned long kFrameFadeStepMs = 12;
constexpr unsigned long kModeFadeStepMs = 10;
constexpr unsigned long kStatusPrintMs = 1000;

constexpr uint8_t kMaxPwm = 255;
constexpr uint8_t kBrightness = (kMaxPwm * kBrightnessPercent) / 100;
constexpr uint8_t kModeFadeStep = 12;
constexpr uint8_t kFrameFadeStep = 24;

enum class AnimationMode : uint8_t {
  Off = 0,
  Chase,
  Fill,
  BlinkAll,
  PingPong,
  Sparkle,
  Count
};

enum class TransitionState : uint8_t {
  Idle,
  FadeOut,
  FadeIn
};

AnimationMode currentMode = AnimationMode::Off;
AnimationMode pendingMode = AnimationMode::Off;
TransitionState transitionState = TransitionState::Idle;

uint8_t ledLevels[kLedCount] = {};
uint8_t targetLevels[kLedCount] = {};
uint8_t transitionLevel = 0;
uint8_t frame = 0;
unsigned long lastFrameAt = 0;
unsigned long lastFrameFadeAt = 0;
unsigned long lastModeFadeAt = 0;
unsigned long lastStatusAt = 0;

bool rawButtonPressed() {
  const int rawLevel = digitalRead(BUTTON_PIN);
  return kButtonActiveHigh ? rawLevel == HIGH : rawLevel == LOW;
}

void writeLed(size_t index, uint8_t level) {
  if (index < kLedCount) {
    analogWrite(kLedPins[index], level);
  }
}

void clearTargets() {
  for (size_t i = 0; i < kLedCount; ++i) {
    targetLevels[i] = 0;
  }
}

void setTargetLed(size_t index, bool enabled) {
  if (index < kLedCount) {
    targetLevels[index] = enabled ? kBrightness : 0;
  }
}

void setAllTargets(bool enabled) {
  for (size_t i = 0; i < kLedCount; ++i) {
    setTargetLed(i, enabled);
  }
}

void clearLedsNow() {
  clearTargets();
  for (size_t i = 0; i < kLedCount; ++i) {
    ledLevels[i] = 0;
    writeLed(i, 0);
  }
}

const char* modeName(AnimationMode mode) {
  switch (mode) {
    case AnimationMode::Off:
      return "Off";
    case AnimationMode::Chase:
      return "Chase";
    case AnimationMode::Fill:
      return "Fill";
    case AnimationMode::BlinkAll:
      return "BlinkAll";
    case AnimationMode::PingPong:
      return "PingPong";
    case AnimationMode::Sparkle:
      return "Sparkle";
    case AnimationMode::Count:
      return "Unknown";
  }

  return "Unknown";
}

AnimationMode nextAnimationMode() {
  uint8_t nextMode = static_cast<uint8_t>(currentMode) + 1;
  if (nextMode >= static_cast<uint8_t>(AnimationMode::Count)) {
    nextMode = static_cast<uint8_t>(AnimationMode::Chase);
  }
  return static_cast<AnimationMode>(nextMode);
}

void resetAnimationTiming() {
  frame = 0;
  lastFrameAt = 0;
  lastFrameFadeAt = 0;
}

void startModeTransition(AnimationMode nextMode) {
  pendingMode = nextMode;
  transitionState = TransitionState::FadeOut;
  transitionLevel = kBrightness;
  lastModeFadeAt = 0;
  Serial.printf("Next mode=%u %s\n", static_cast<uint8_t>(nextMode), modeName(nextMode));
}

void handleButton() {
  static bool lastRawState = rawButtonPressed();
  static bool stableState = lastRawState;
  static unsigned long lastRawChangeAt = millis();
  static unsigned long pressedAt = 0;
  static bool longPressHandled = false;

  const bool currentRawState = rawButtonPressed();
  const unsigned long now = millis();

  if (currentRawState != lastRawState) {
    lastRawState = currentRawState;
    lastRawChangeAt = now;
  }

  if ((now - lastRawChangeAt) < kDebounceMs || stableState == currentRawState) {
    if (stableState && !longPressHandled && (now - pressedAt) >= kLongPressMs && transitionState == TransitionState::Idle) {
      longPressHandled = true;
      startModeTransition(AnimationMode::Off);
    }
    return;
  }

  stableState = currentRawState;
  Serial.printf("Button=%d\n", stableState);

  if (stableState) {
    pressedAt = now;
    longPressHandled = false;
    return;
  }

  if (!longPressHandled && transitionState == TransitionState::Idle) {
    startModeTransition(nextAnimationMode());
  }
}

bool frameDue(unsigned long intervalMs) {
  const unsigned long now = millis();
  if ((now - lastFrameAt) < intervalMs) {
    return false;
  }

  lastFrameAt = now;
  return true;
}

void updateChaseTargets() {
  if (!frameDue(120)) {
    return;
  }

  clearTargets();
  setTargetLed(frame % kLedCount, true);
  frame = (frame + 1) % kLedCount;
}

void updateFillTargets() {
  if (!frameDue(180)) {
    return;
  }

  clearTargets();
  const uint8_t litCount = frame % (kLedCount + 1);
  for (uint8_t i = 0; i < litCount; ++i) {
    setTargetLed(i, true);
  }
  frame = (frame + 1) % (kLedCount + 1);
}

void updateBlinkAllTargets() {
  if (!frameDue(300)) {
    return;
  }

  setAllTargets((frame % 2) == 0);
  ++frame;
}

void updatePingPongTargets() {
  if (!frameDue(120)) {
    return;
  }

  constexpr uint8_t kPathLength = (kLedCount * 2) - 2;
  const uint8_t position = frame % kPathLength;
  const uint8_t ledIndex = position < kLedCount ? position : kPathLength - position;

  clearTargets();
  setTargetLed(ledIndex, true);
  ++frame;
}

void updateSparkleTargets() {
  if (!frameDue(90)) {
    return;
  }

  clearTargets();
  setTargetLed(random(kLedCount), true);
  if ((frame % 3) == 0) {
    setTargetLed(random(kLedCount), true);
  }
  ++frame;
}

void updateAnimationTargets() {
  switch (currentMode) {
    case AnimationMode::Off:
      clearTargets();
      break;
    case AnimationMode::Chase:
      updateChaseTargets();
      break;
    case AnimationMode::Fill:
      updateFillTargets();
      break;
    case AnimationMode::BlinkAll:
      updateBlinkAllTargets();
      break;
    case AnimationMode::PingPong:
      updatePingPongTargets();
      break;
    case AnimationMode::Sparkle:
      updateSparkleTargets();
      break;
    case AnimationMode::Count:
      currentMode = AnimationMode::Off;
      clearTargets();
      break;
  }
}

void fadeRenderedLevelsTowardTargets() {
  const unsigned long now = millis();
  if ((now - lastFrameFadeAt) < kFrameFadeStepMs) {
    return;
  }
  lastFrameFadeAt = now;

  for (size_t i = 0; i < kLedCount; ++i) {
    if (ledLevels[i] < targetLevels[i]) {
      ledLevels[i] = min<uint8_t>(ledLevels[i] + kFrameFadeStep, targetLevels[i]);
    } else if (ledLevels[i] > targetLevels[i]) {
      ledLevels[i] = (ledLevels[i] > kFrameFadeStep) ? max<uint8_t>(ledLevels[i] - kFrameFadeStep, targetLevels[i]) : targetLevels[i];
    }

    writeLed(i, ledLevels[i]);
  }
}

void renderTransitionLevel() {
  for (size_t i = 0; i < kLedCount; ++i) {
    writeLed(i, transitionLevel);
  }
}

void handleModeTransition() {
  if (transitionState == TransitionState::Idle) {
    return;
  }

  const unsigned long now = millis();
  if ((now - lastModeFadeAt) < kModeFadeStepMs) {
    return;
  }
  lastModeFadeAt = now;

  if (transitionState == TransitionState::FadeOut) {
    if (transitionLevel > kModeFadeStep) {
      transitionLevel -= kModeFadeStep;
      renderTransitionLevel();
      return;
    }

    currentMode = pendingMode;
    resetAnimationTiming();
    clearLedsNow();
    transitionState = TransitionState::FadeIn;
    transitionLevel = 0;
    Serial.printf("Mode=%u %s\n", static_cast<uint8_t>(currentMode), modeName(currentMode));
    return;
  }

  if (transitionLevel < kBrightness) {
    transitionLevel = min<uint8_t>(transitionLevel + kModeFadeStep, kBrightness);
    renderTransitionLevel();
    return;
  }

  clearLedsNow();
  transitionState = TransitionState::Idle;
}

void printStatus() {
  const unsigned long now = millis();
  if ((now - lastStatusAt) < kStatusPrintMs) {
    return;
  }

  lastStatusAt = now;
  Serial.printf(
    "Status: button=%d mode=%u %s brightness=%u%% transition=%u\n",
    rawButtonPressed(),
    static_cast<uint8_t>(currentMode),
    modeName(currentMode),
    kBrightnessPercent,
    static_cast<uint8_t>(transitionState)
  );
}
}  // namespace

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  for (size_t i = 0; i < kLedCount; ++i) {
    pinMode(kLedPins[i], OUTPUT);
  }
  clearLedsNow();

  Serial.begin(115200);
  delay(500);
  randomSeed(esp_random());

  Serial.println("LED workshop started");
  Serial.printf("Mode=0 %s brightness=%u%%\n", modeName(currentMode), kBrightnessPercent);
}

void loop() {
  handleButton();
  handleModeTransition();

  if (transitionState == TransitionState::Idle) {
    updateAnimationTargets();
    fadeRenderedLevelsTowardTargets();
  }

  printStatus();
  delay(1);
}
