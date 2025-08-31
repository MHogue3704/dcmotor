# Wiring Guide: ESP32-C3 with SH1106 OLED & Rotary Encoder Module

This guide details how to connect your ESP32-C3 to the SH1106 OLED module with an integrated rotary encoder.

## Components

*   ESP32-C3 Development Board
*   SH1106 OLED Module with Rotary Encoder
*   Jumper Wires

## Pinout Identification

The pins on your module are labeled as follows:
`VCC`, `GND`, `SCL`, `SDA`, `TRB`, `TRA`, `PSH`, `BAK`, `CON`

*   **VCC/GND**: Power pins.
*   **SCL/SDA**: I2C communication pins for the display.
*   **TRA/TRB**: Rotary encoder pins A and B.
*   **PSH**: Rotary encoder push-button.
*   **BAK**: "Back" button.
*   **CON**: "Confirm" button.

## Wiring Connections

Connect the pins from the OLED module to your ESP32-C3 as follows. These pins are chosen to avoid conflict with the motor driver pins.

*   **OLED `VCC`** → **ESP32-C3 `3V3`**
*   **OLED `GND`** → **ESP32-C3 `GND`**
*   **OLED `SCL`** → **ESP32-C3 `GPIO5`** (I2C Clock)
*   **OLED `SDA`** → **ESP32-C3 `GPIO6`** (I2C Data)
*   **OLED `TRA`** → **ESP32-C3 `GPIO7`** (Encoder Pin A)
*   **OLED `TRB`** → **ESP32-C3 `GPIO8`** (Encoder Pin B)
*   **OLED `PSH`** → **ESP32-C3 `GPIO9`** (Encoder Push Button)
*   **OLED `BAK`** → **ESP32-C3 `GPIO10`** (Back Button)
*   **OLED `CON`** → **ESP32-C3 `GPIO18`** (Confirm Button)

## Summary Diagram

```
ESP32-C3                SH1106 Module
--------                -------------
  3V3  ────────────────→   VCC
  GND  ────────────────→   GND
GPIO5  ────────────────→   SCL
GPIO6  ────────────────→   SDA
GPIO7  ────────────────→   TRA
GPIO8  ────────────────→   TRB
GPIO9  ────────────────→   PSH
GPIO10 ────────────────→   BAK
GPIO18 ────────────────→   CON
```
