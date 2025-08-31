# Wiring Guide: ESP32 with SH1106 OLED & Rotary Encoder Module

This guide details how to connect your ESP32 to the SH1106 OLED module with an integrated rotary encoder.

## Components

*   ESP32 Development Board
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

Connect the pins from the OLED module to your ESP32 as follows. These pins are chosen to avoid conflict with the motor driver pins.

*   **OLED `VCC`** → **ESP32 `3V3`**
*   **OLED `GND`** → **ESP32 `GND`**
*   **OLED `SCL`** → **ESP32 `GPIO22`** (I2C Clock)
*   **OLED `SDA`** → **ESP32 `GPIO21`** (I2C Data)
*   **OLED `TRA`** → **ESP32 `GPIO25`** (Encoder Pin A)
*   **OLED `TRB`** → **ESP32 `GPIO26`** (Encoder Pin B)
*   **OLED `PSH`** → **ESP32 `GPIO27`** (Encoder Push Button)
*   **OLED `BAK`** → **ESP32 `GPIO32`** (Back Button)
*   **OLED `CON`** → **ESP32 `GPIO33`** (Confirm Button)

## Summary Diagram

```
ESP32                   SH1106 Module
--------                -------------
  3V3  ────────────────→   VCC
  GND  ────────────────→   GND
GPIO22 ────────────────→   SCL
GPIO21 ────────────────→   SDA
GPIO25 ────────────────→   TRA
GPIO26 ────────────────→   TRB
GPIO27 ────────────────→   PSH
GPIO32 ────────────────→   BAK
GPIO33 ────────────────→   CON
```
