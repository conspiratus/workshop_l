# LCD1602 Large Heart (2 Characters Wide)

## Goal

This example demonstrates how two custom LCD characters can be combined into one larger image.

Instead of creating a single 5×8 symbol, we create:

* Character 0 → left half of the heart
* Character 1 → right half of the heart

When displayed side by side, they form one large heart.

---

## Heart Design

### Full 10×8 Image

```text
.##....##.
####..####
##########
##########
.########.
..######..
...####...
....##....
```

---

## Character 0 (Left Half)

```text
.##..
####.
#####
#####
.####
..###
...##
....#
```

### Binary Representation

```cpp
byte heartLeft[8] = {
  B01100,
  B11110,
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001
};
```

---

## Character 1 (Right Half)

```text
..##.
.####
#####
#####
####.
###..
##...
#....
```

### Binary Representation

```cpp
byte heartRight[8] = {
  B00110,
  B01111,
  B11111,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
};
```

---

## Loading Characters into LCD Memory

```cpp
lcd.createChar(0, heartLeft);
lcd.createChar(1, heartRight);
```

The LCD now knows:

| Character ID | Image                   |
| ------------ | ----------------------- |
| 0            | Left half of the heart  |
| 1            | Right half of the heart |

---

## Displaying the Heart

```cpp
lcd.setCursor(0, 0);

lcd.write(byte(0));
lcd.write(byte(1));
```

Result:

```text
♥
```

(Displayed as a large heart built from two custom LCD characters.)

---

## How It Works

1. Draw a picture.
2. Convert pixels into binary rows.
3. Store each half as a custom LCD character.
4. Assign each character an ID.
5. Display the IDs side by side.

```text
Drawing
    ↓
Pixels
    ↓
Binary
    ↓
Custom Characters
    ↓
LCD Image
```

---

## Engineering Idea

A large image can be built from smaller reusable blocks.

This is the same principle used in:

* LCD displays
* LED matrices
* Video games
* Computer graphics
* Modern displays

Every element has an address, and software decides what appears at that address.
