# ESP32 DC Motor Controller with OLED Interface

This project is a sophisticated motor controller for a 12V DC motor, built using an ESP32 microcontroller. It features a user-friendly interface with an SH1106 OLED display and a rotary encoder for precise speed adjustments, making it ideal for applications requiring fine-tuned motor control.

## Features

*   **Precise Speed Control:** Utilizes PWM to adjust the motor's speed from 0% to 100%.
*   **OLED Display Interface:** A 1.3-inch SH1106 OLED screen displays the motor's status (ON/OFF) and current speed.
*   **Rotary Encoder Input:** A rotary encoder with a push-button allows for intuitive speed adjustments and control.
*   **Save & Revert Speed Settings:** A "Confirm" button saves the desired speed, while a "Back" button reverts to the previously saved setting.
*   **Robust Motor Driver:** Employs the L298N H-Bridge to safely and efficiently drive the DC motor.
*   **Built with PlatformIO:** The project is configured for the PlatformIO IDE, ensuring easy dependency management and compilation.

## Hardware Components

| Component                               | Purpose                                     |
| --------------------------------------- | ------------------------------------------- |
| ESP32 Development Board              | Main microcontroller for logic and control. |
| L298N H-Bridge Motor Driver             | Drives the high-current DC motor.           |
| 12V DC Geared Motor (e.g., 3 RPM)       | The motor being controlled.                 |
| SH1106 1.3" OLED Module with Encoder    | User interface for display and input.       |
| CA-1235 Buck Converter Module           | Steps down 12V to 5V for the ESP32.         |
| 12V DC Power Supply                     | Powers the L298N driver and the motor.      |
| Jumper Wires                            | For making connections.                     |

## Wiring Instructions

This project requires wiring for both the motor driver and the OLED display module. Detailed, step-by-step guides for each are provided in the repository:

*   **For the L298N Motor Driver:** Please refer to the [**`WIRING.md`**](./WIRING.md) file.
*   **For the SH1106 OLED Module:** Please refer to the [**`OLED_WIRING.md`**](./OLED_WIRING.md) file.

**Crucial:** Ensure you have a common ground (`GND`) connection between the ESP32-C3, the L298N module, and the OLED module for stable operation.

## Software Setup & Installation

This project is managed using PlatformIO.

1.  **Install PlatformIO:** If you haven't already, install [PlatformIO IDE for VS Code](https://platformio.org/install/ide?install=vscode).
2.  **Clone the Repository:**
    ```bash
    git clone https://github.com/MHogue3704/dcmotor.git
    cd dcmotor
    ```
3.  **Open in PlatformIO:** Open the cloned folder in VS Code with the PlatformIO extension installed.
4.  **Build & Upload:** PlatformIO will automatically detect the `platformio.ini` file and download the required libraries (`U8g2`, `RotaryEncoder`, `OneButton`). Once the dependencies are installed, build and upload the project to your ESP32.

## How to Use

Once the hardware is wired and the firmware is uploaded, the controller is ready to use:

1.  **Power On:** Connect the ESP32 via USB and turn on the 12V power supply for the motor.
2.  **View Display:** The OLED screen will light up, showing the motor status and speed.
3.  **Adjust Speed:** Turn the rotary encoder knob to increase or decrease the motor speed. The display will show `(Unsaved)` to indicate the change is temporary.
4.  **Save Speed:** Press the **"Confirm"** button to save the new speed. The `(Unsaved)` message will disappear.
5.  **Revert Speed:** If you make an adjustment you don't want, press the **"Back"** button to revert to the last saved speed.
6.  **Toggle Motor:** Click the **rotary encoder's built-in button** to toggle the motor ON or OFF.

## Code Overview

The main logic is contained in `src/main.cpp`:

*   **Pin Definitions:** All GPIO pin assignments for the motor and OLED module are at the top of the file.
*   **`setup()`:** Initializes serial communication, pin modes, PWM channels, the OLED display, and button handlers.
*   **`loop()`:** Continuously checks for input from the encoder and buttons.
*   **`drawScreen()`:** Renders the user interface on the OLED display.
*   **`updateMotor()`:** Applies the current speed value to the motor via PWM.
*   **Input Handlers (`checkEncoder`, `handle...Click`)**: These functions process input from the rotary encoder and buttons to modify the motor's state.
