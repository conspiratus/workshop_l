# Workshop L

This repository stores multiple workshop projects in one place.

## Structure

- `workshops/` contains standalone PlatformIO projects grouped by topic name.
- Each workshop keeps its own `platformio.ini`, `src/`, `include/`, `lib/`, and `test/`.

## Current Workshops

- `workshops/LCD1602` - ESP32-C3 example for a 16x2 I2C LCD.

## Adding A New Workshop

1. Create a new folder under `workshops/<TOPIC_NAME>/`.
2. Initialize a new PlatformIO project inside that folder.
3. Commit the new workshop to this repository.
