#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <OneButton.h>

// --- Pin Definitions ---

// L298N Motor Driver Pins
const int MOTOR_IN1 = 13;
const int MOTOR_IN2 = 12;
const int MOTOR_ENA = 14;

// SH1106 OLED & Encoder Module Pins
const int OLED_SCL = 22;
const int OLED_SDA = 21;
const int ENC_A = 25;
const int ENC_B = 26;
const int ENC_BTN = 27;
const int BACK_BTN = 32;
const int CONFIRM_BTN = 33;

// --- PWM Configuration ---
const int PWM_CHANNEL = 0;
const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8; // 0-255

// --- Global Variables ---
int motorSpeed = 60; // Current PWM value (0-255)
int savedSpeed = 60; // Speed that is "confirmed"
bool motorRunning = true;
const int MIN_SPEED = 0;
const int MAX_SPEED = 255;

// --- Object Declarations ---

// U8g2 OLED Display
// Using U8G2_SH1106_128X64_NONAME_F_HW_I2C for hardware I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Rotary Encoder
RotaryEncoder encoder(ENC_A, ENC_B, RotaryEncoder::LatchMode::FOUR3);

// Buttons
OneButton encoderButton(ENC_BTN, true); // 'true' for active low
OneButton backButton(BACK_BTN, true);
OneButton confirmButton(CONFIRM_BTN, true);

// --- Function Prototypes ---
void drawScreen();
void updateMotor();
void checkEncoder();
void handleEncoderClick();
void handleBackClick();
void handleConfirmClick();

void setup() {
  Serial.begin(115200);
  Serial.println("Motor Controller with OLED Initializing...");

  // Motor Pin Setup
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, HIGH); // Set direction forward
  digitalWrite(MOTOR_IN2, LOW);

  // PWM Setup
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_ENA, PWM_CHANNEL);

  // I2C and Display Setup
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!u8g2.begin()) {
    Serial.println("U8g2 init failed!");
    while (1);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr); // Set a nice font

  // Button Setup
  encoderButton.attachClick(handleEncoderClick);
  backButton.attachClick(handleBackClick);
  confirmButton.attachClick(handleConfirmClick);

  // Initial State
  updateMotor();
  drawScreen();

  Serial.println("Initialization complete.");
}

void loop() {
  // Check for inputs
  checkEncoder();
  encoderButton.tick();
  backButton.tick();
  confirmButton.tick();

  // The loop continuously updates inputs and redraws the screen.
  // For this simple case, we could redraw only on change, but this is fine.
  drawScreen();
  delay(100); // Small delay to prevent screen flicker and CPU overload
}

// --- Core Functions ---

void drawScreen() {
  u8g2.clearBuffer();
  
  // Title
  u8g2.drawStr(0, 10, "Motor Speed Control");
  u8g2.drawHLine(0, 14, 128);

  // Motor Status
  u8g2.drawStr(0, 30, "Status:");
  u8g2.drawStr(50, 30, motorRunning ? "ON" : "OFF");

  // Speed Value
  u8g2.drawStr(0, 45, "Speed:");
  char speedStr[5];
  sprintf(speedStr, "%d", motorSpeed);
  u8g2.drawStr(50, 45, speedStr);

  // Display saved speed if different
  if (motorSpeed != savedSpeed) {
    u8g2.drawStr(80, 45, "(Unsaved)");
  }

  // Instructions
  u8g2.drawStr(0, 60, "Click knob=On/Off | CON=Save");

  u8g2.sendBuffer();
}

void updateMotor() {
  if (motorRunning) {
    ledcWrite(PWM_CHANNEL, motorSpeed);
  } else {
    ledcWrite(PWM_CHANNEL, 0);
  }
  Serial.print("Motor state: ");
  Serial.print(motorRunning ? "ON" : "OFF");
  Serial.print(", Speed PWM: ");
  Serial.println(motorSpeed);
}

// --- Input Handling ---

void checkEncoder() {
  static int lastPos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();

  if (newPos != lastPos) {
    if (newPos > lastPos) {
      motorSpeed = min(motorSpeed + 5, MAX_SPEED);
    } else {
      motorSpeed = max(motorSpeed - 5, MIN_SPEED);
    }
    lastPos = newPos;
    updateMotor(); // Update motor speed immediately on change
  }
}

void handleEncoderClick() {
  motorRunning = !motorRunning; // Toggle motor state
  Serial.println("Encoder button clicked. Toggling motor.");
  updateMotor();
}

void handleBackClick() {
  // Revert speed to the last saved value
  motorSpeed = savedSpeed;
  Serial.println("Back button clicked. Reverting to saved speed.");
  updateMotor();
}

void handleConfirmClick() {
  // Save the current speed
  savedSpeed = motorSpeed;
  Serial.println("Confirm button clicked. Speed saved.");
  // You could also save this to EEPROM/Preferences for persistence
}