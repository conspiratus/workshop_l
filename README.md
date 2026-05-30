# Workshop L

This repository stores multiple workshop projects in one place.

## Structure

- `workshops/` contains standalone PlatformIO projects grouped by topic name.
- Each workshop keeps its own `platformio.ini`, `src/`, `include/`, `lib/`, and `test/`.
- The repository root keeps a small PlatformIO wrapper so opening the repo in VS Code still works.

## Current Workshops

- `workshops/LCD1602` - ESP32-C3 example for a 16x2 I2C LCD.

## PlatformIO

- Open the repository root in VS Code and PlatformIO will use the root `platformio.ini` plus `src/main.cpp` as an entry point.
- The active workshop is currently `workshops/LCD1602`.
- The workshop config does not hardcode a serial port, so the clone is not tied to one machine.
- The root `src/main.cpp` currently includes `workshops/LCD1602/src/main.cpp`.
- If upload autodetection is unstable on macOS with ESP32-C3, use `platformio run -t upload --upload-port /dev/tty.usbmodem...`.

## Adding A New Workshop

1. Create a new folder under `workshops/<TOPIC_NAME>/`.
2. Initialize a new PlatformIO project inside that folder.
3. Commit the new workshop to this repository.
