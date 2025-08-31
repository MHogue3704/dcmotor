# Wiring Guide: ESP32 with L298N DC Motor Driver

This guide details how to connect your ESP32 to an L298N H-Bridge module to control a 12V DC motor.

## Components

*   ESP32 Development Board
*   L298N H-Bridge Motor Driver Module
*   12V DC Motor
*   12V Power Supply for the motor
*   Jumper Wires

## Wiring Connections

### 1. L298N Power Connections

The L298N needs power for both its internal logic and the motor.

*   **L298N `+12V` Terminal** → **12V Power Supply `+` (Positive)**
*   **L298N `GND` Terminal** → **12V Power Supply `-` (Negative)** AND **ESP32 `GND` Pin** (this is crucial for a common ground reference).
*   **L298N `+5V` Terminal**: This is typically an *output* if the jumper next to it is in place. Do not connect anything here for now.

**L298N Jumper:**
*   The L298N module has a jumper (often labeled `5V_EN` or similar). **Keep this jumper on.** This enables the onboard 7805 voltage regulator, which creates 5V for the L298N's logic from your 12V motor supply.

### 2. Control Signal Connections (ESP32 to L298N)

These pins control the motor's speed and direction.

*   **ESP32 `GPIO13`** → **L298N `IN1`** (Motor Direction Pin 1)
*   **ESP32 `GPIO12`** → **L298N `IN2`** (Motor Direction Pin 2)
*   **ESP32 `GPIO14`** → **L298N `ENA`** (Motor Speed Control - PWM)

*Note: We are using Motor A outputs. If you were using Motor B, you would use `IN3`, `IN4`, and `ENB`.*

### 3. Motor Connections (L298N to DC Motor)

*   **L298N `OUT1` Terminal** → **DC Motor Terminal 1**
*   **L298N `OUT2` Terminal** → **DC Motor Terminal 2**

*If the motor spins in the opposite direction of what you expect, simply swap the wires on `OUT1` and `OUT2`.*

## Summary Diagram

```
// Power
12V PSU (+) -------> L298N +12V
12V PSU (-) -------> L298N GND
                  |
                  '-> ESP32 GND

// Control Signals
ESP32 GPIO13 ----> L298N IN1
ESP32 GPIO12 ----> L298N IN2
ESP32 GPIO14 ----> L298N ENA

// Motor
L298N OUT1 --------> DC Motor
L298N OUT2 --------> DC Motor
```
