# Workshop L

This repository stores multiple workshop projects in one place.

## Setup

See [SETUP.md](SETUP.md) for instructions on installing VS Code, PlatformIO IDE
and Codex, and creating a project for the Ai-Thinker ESP-C3-M1-I-Kit board.

## Structure

- `workshops/` contains standalone PlatformIO projects grouped by topic name.
- Each workshop keeps its own `platformio.ini`, `src/`, `include/`, `lib/`, and `test/`.
- The repository root keeps a small PlatformIO wrapper so opening the repo in VS Code still works.

## Current Workshops

- `workshops/LCD1602` - ESP32-C3 example for a 16x2 I2C LCD.
- `workshops/LED` - ESP32-C3 example with five LED animation modes.
- `workshops/Transistor` - ESP32-C3 example with a touch button and transistor-switched motor.

## Selecting A Workshop

When the repository root is open in VS Code, PlatformIO builds the root
`platformio.ini` and uses `src/main.cpp` as the entry point.

Select the active workshop by leaving exactly one `#include` uncommented in
`src/main.cpp`:

```cpp
// LCD1602 is active
#include "../workshops/LCD1602/src/main.cpp"
// #include "../workshops/LED/src/main.cpp"
// #include "../workshops/Transistor/src/main.cpp"
```

To compile another workshop, comment the current line and uncomment the required
one:

```cpp
// LED is active
// #include "../workshops/LCD1602/src/main.cpp"
#include "../workshops/LED/src/main.cpp"
// #include "../workshops/Transistor/src/main.cpp"
```

Only one workshop must be active at a time. Otherwise, the compiler will find
multiple definitions of `setup()` and `loop()`.

If the selected workshop requires a library, add the same dependency to the
root `platformio.ini`. For example:

```ini
lib_deps =
  marcoschwartz/LiquidCrystal_I2C@^1.1.4
```

## Build And Upload From The Repository Root

Build the workshop selected in `src/main.cpp`:

```sh
pio run
```

Upload it to the board:

```sh
pio run -t upload
```

Open Serial Monitor:

```sh
pio device monitor
```

Upload and then open Serial Monitor:

```sh
pio run -t upload && pio device monitor
```

The same actions are available in the PlatformIO toolbar in VS Code: `Build`,
`Upload`, and `Serial Monitor`.

## Build A Workshop Directly

Each directory under `workshops/` is also a standalone PlatformIO project. It
can be built without changing the root `src/main.cpp`.

Examples:

```sh
pio run -d workshops/LCD1602
pio run -d workshops/LED
pio run -d workshops/Transistor
```

Upload a specific workshop:

```sh
pio run -d workshops/LED -t upload
```

Open its Serial Monitor:

```sh
pio device monitor -d workshops/LED
```

If automatic port detection fails on macOS, specify the port explicitly:

```sh
pio run -d workshops/LED -t upload --upload-port /dev/cu.usbmodemXXXX
pio device monitor -d workshops/LED --port /dev/cu.usbmodemXXXX
```

## Adding A New Workshop

1. Create a new folder under `workshops/<TOPIC_NAME>/`.
2. Initialize a new PlatformIO project inside that folder.
3. Add the workshop include to the root `src/main.cpp` in a commented state.
4. Add any required libraries to both the workshop and root `platformio.ini`.
5. Verify the standalone build with `pio run -d workshops/<TOPIC_NAME>`.
6. Commit the new workshop to this repository.
