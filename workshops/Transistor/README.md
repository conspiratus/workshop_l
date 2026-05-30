# Transistor Workshop

ESP32-C3 example:

- a touch button module is connected to one digital input
- a transistor switch that powers a motor is connected to one digital output

## Default Pins

- `GPIO4` -> touch sensor output
- `GPIO5` -> transistor control

## Behavior

- when the button input is `HIGH`, the motor output is `HIGH`
- when the button input is `LOW`, the motor output is `LOW`

## Wiring Assumption

This example assumes the touch module outputs digital `HIGH` when touched.
If your module works inverted, change the logic in `src/main.cpp`.
