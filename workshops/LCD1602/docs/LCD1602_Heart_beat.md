# LCD1602 I2C Display with ESP32-C3

## Goal

In this workshop we will:

* Connect an LCD1602 display to an ESP32-C3
* Display text on the screen
* Create custom symbols
* Combine multiple symbols into one larger image
* Create a simple animation using custom characters

---

# Hardware

## Components

* ESP32-C3
* LCD1602 with I2C backpack
* Jumper wires

---

## Wiring

| LCD1602 | ESP32-C3 |
| ------- | -------- |
| VCC     | 5V       |
| GND     | GND      |
| SDA     | GPIO 8   |
| SCL     | GPIO 9   |

---

# Installing the Library

PlatformIO:

```bash
pio pkg install --library "marcoschwartz/LiquidCrystal_I2C"
```

---

# First Program

The simplest example displays text on the LCD.

```cpp
lcd.setCursor(0, 0);
lcd.print("HELLO");

lcd.setCursor(0, 1);
lcd.print("ROBERT");
```

Result:

```text
HELLO
ROBERT
```

---

# Understanding Coordinates

The LCD contains:

```text
16 columns
2 rows
```

Coordinates start from:

```text
(0,0)
```

which is the upper-left corner.

Example:

```cpp
lcd.setCursor(5, 0);
lcd.print("HELLO");
```

Result:

```text
     HELLO
```

---

# Custom Characters

The LCD allows us to create up to:

```text
8 custom characters
```

Each character is:

```text
5 × 8 pixels
```

Example:

```text
.....
.#.#.
#####
#####
#####
.###.
..#..
.....
```

Each row becomes binary data:

```cpp
byte heart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
```

Load it into memory:

```cpp
lcd.createChar(0, heart);
```

Display it:

```cpp
lcd.write(byte(0));
```

---

# Building Large Images

A single LCD character is only 5 pixels wide.

To create larger images we can combine multiple characters.

Example:

```text
+-----+-----+
|  0  |  1  |
+-----+-----+
```

Character 0 contains the left half.

Character 1 contains the right half.

Displayed together they create a larger image.

---

# Animated Heart

This example creates a beating heart.

The animation works by replacing the graphics stored in custom character memory.

Small heart:

```cpp
lcd.createChar(0, smallHeartLeft);
lcd.createChar(1, smallHeartRight);
```

Big heart:

```cpp
lcd.createChar(0, bigHeartLeft);
lcd.createChar(1, bigHeartRight);
```

Display:

```cpp
lcd.setCursor(0, 1);
lcd.write(byte(0));
lcd.write(byte(1));
```

The IDs stay the same:

```text
0
1
```

but the images stored under those IDs change.

This creates animation.

---

# Main Loop

```cpp
showSmallHeart();
delay(350);

showBigHeart();
delay(180);

showSmallHeart();
delay(350);

showBigHeart();
delay(180);

delay(700);
```
