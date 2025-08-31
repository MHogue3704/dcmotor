# Power Distribution Wiring Guide

This guide explains how to power all components (ESP32-C3, L298N, and OLED) from a single 12V power supply using a CA-1235 buck converter. This setup is essential for a stable, self-contained project without relying on USB power for the microcontroller.

## Components

*   **12V DC Power Supply:** The main power source for the entire project.
*   **CA-1235 Buck Converter:** A step-down module to convert 12V from the power supply into a stable 5V for the ESP32.
*   **ESP32-C3, L298N, OLED Module:** The core components of the project.

## Step 1: Set the Buck Converter to 5V

**CRITICAL STEP:** Before connecting the buck converter to your ESP32, you MUST ensure its output is set to 5V.

1.  **Connect Input:** Connect your 12V power supply to the `IN+` and `IN-` terminals of the CA-1235 module.
2.  **Measure Output:** Use a multimeter to measure the voltage across the `OUT+` and `OUT-` terminals.
3.  **Adjust (if necessary):** The CA-1235 often has solder pads or a potentiometer to select the output voltage. Adjust it until your multimeter reads **5V**. If it's already fixed at 5V, confirm this reading.
4.  **Disconnect Power:** Once confirmed, disconnect the 12V power supply.

**Do not proceed until you have confirmed a stable 5V output.**

## Step 2: Wiring Diagram

The goal is to distribute power from the 12V supply to the L298N directly and to the ESP32/OLED via the 5V buck converter.

1.  **12V Power Supply to Components:**
    *   **PSU `+` (Positive)** → Connect to **both**:
        *   L298N `+12V` terminal.
        *   CA-1235 `IN+` terminal.
    *   **PSU `-` (Negative/GND)** → Connect to **both**:
        *   L298N `GND` terminal.
        *   CA-1235 `IN-` terminal.

2.  **Buck Converter (5V Out) to ESP32:**
    *   **CA-1235 `OUT+`** → **ESP32-C3 `5V` pin**.
    *   **CA-1235 `OUT-`** → **ESP32-C3 `GND` pin**.

3.  **Common Ground:**
    *   A common ground is essential. By connecting the 12V PSU's negative line to both the L298N and the buck converter's input, and the buck converter's output ground to the ESP32, all components will share the same ground reference.

## Summary Diagram

```
                      +---------------------+
12V PSU (+) --------->| L298N +12V          |
                      |                     |
                      +---------------------+

                      +---------------------+
12V PSU (+) --------->| CA-1235 IN+         |
                      |         OUT+ (5V) --+--> ESP32-C3 (5V Pin)
                      |                     |
12V PSU (-) --+------>| CA-1235 IN-         |
              |       |         OUT- (GND)--+--> ESP32-C3 (GND Pin)
              |       +---------------------+
              |
              |       +---------------------+
              +------>| L298N GND           |
                      |                     |
                      +---------------------+
```

The OLED display is powered by the ESP32's 3.3V regulator, so its wiring does not change. By powering the ESP32 through its `5V` pin, its onboard regulator will provide the `3.3V` needed for the OLED and its own logic.
