# LCD1602 Small Heart (1 Custom Character)

## Goal

This example shows how to create one custom heart symbol for an LCD1602 display.

One LCD custom character has a size of:

```text
5 × 8 pixels
```

So the whole heart fits inside one character cell.

---

## Heart Design

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

---

## Binary Representation

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

---

## Loading the Character into LCD Memory

```cpp
lcd.createChar(0, heart);
```

The LCD now knows:

| Character ID | Image       |
| ------------ | ----------- |
| 0            | Small heart |

---

## Displaying the Heart

```cpp
lcd.setCursor(0, 0);
lcd.write(byte(0));
```
