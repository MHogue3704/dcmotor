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
int motorSpeed = 65; // Current PWM value (0-255)
int savedSpeed = 65; // Speed that is "confirmed"
bool motorRunning = true;
const int MIN_SPEED = 0;
const int MAX_SPEED = 255;

// --- Object Declarations ---

// Onboard OLED (SSD1306 or SH1106)
// Example: onboard SSD1306 at 0x3C, external SH1106 at 0x3C or 0x3D
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C sh1106_u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // External SH1106
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C onboard_u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // Onboard OLED (change type if needed)

// Rotary Encoder
RotaryEncoder encoder(ENC_A, ENC_B, RotaryEncoder::LatchMode::FOUR0);

// Buttons
OneButton encoderButton(ENC_BTN, true); // 'true' for active low
OneButton backButton(BACK_BTN, true);
OneButton confirmButton(CONFIRM_BTN, true);

// --- Function Prototypes ---
void drawOnboardScreen();
void drawSH1106Screen();
void updateMotor();
void checkEncoder();
void handleEncoderClick();
void handleBackClick();
void handleConfirmClick();
void handleLongPressConfirm();

unsigned long totalRunTimeMs = 0;
unsigned long lastRunUpdate = 0;

void setup() {
  // Enable internal pull-ups for rotary encoder pins
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
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
  // Removed onboard OLED initialization
  if (!sh1106_u8g2.begin()) {
    Serial.println("SH1106 OLED init failed!");
    while (1);
  }
  sh1106_u8g2.setFont(u8g2_font_ncenB08_tr);

  // Button Setup
  encoderButton.attachClick(handleEncoderClick);
  backButton.attachClick(handleConfirmClick); // Now increases speed
  confirmButton.attachClick(handleBackClick); // Now decreases speed
  backButton.attachLongPressStart(handleLongPressConfirm);
  confirmButton.attachLongPressStart(handleLongPressConfirm);

  // Initial State
  updateMotor();
  // Removed drawOnboardScreen call
  drawSH1106Screen();

  Serial.println("Initialization complete.");
}

void loop() {
  // Check for inputs
  checkEncoder();
  encoderButton.tick();
  backButton.tick();
  confirmButton.tick();

  // Update running time if motor is running
  if (motorRunning) {
    unsigned long now = millis();
    if (lastRunUpdate == 0) lastRunUpdate = now;
    totalRunTimeMs += (now - lastRunUpdate);
    lastRunUpdate = now;
  } else {
    lastRunUpdate = millis();
  }

  // Removed drawOnboardScreen call
  drawSH1106Screen();
  // Removed delay for maximum encoder polling frequency
}

// --- Core Functions ---

// Removed drawOnboardScreen function

void drawSH1106Screen() {
  sh1106_u8g2.clearBuffer();
  sh1106_u8g2.drawStr(0, 10, "Motor Speed Control");
  sh1106_u8g2.drawHLine(0, 14, 128);

  // Motor Status
  sh1106_u8g2.drawStr(0, 30, "Status:");
  sh1106_u8g2.drawStr(50, 30, motorRunning ? "ON" : "OFF");

  // Speed Value
  sh1106_u8g2.drawStr(0, 45, "Speed:");
  char speedStr[5];
  sprintf(speedStr, "%d", motorSpeed);
  sh1106_u8g2.drawStr(50, 45, speedStr);

  // Display saved speed if different
  if (motorSpeed != savedSpeed) {
    sh1106_u8g2.drawStr(80, 45, "(Unsaved)");
  }

  // Instructions
  sh1106_u8g2.drawStr(0, 60, "Click knob=On/Off | CON=Save");

  sh1106_u8g2.sendBuffer();
}

void updateMotor() {
  if (motorRunning) {
    ledcWrite(PWM_CHANNEL, motorSpeed);
    Serial.print("[DEBUG] ledcWrite PWM_CHANNEL: ");
    Serial.print(PWM_CHANNEL);
    Serial.print(", motorSpeed: ");
    Serial.println(motorSpeed);
  } else {
    ledcWrite(PWM_CHANNEL, 0);
    Serial.print("[DEBUG] ledcWrite PWM_CHANNEL: ");
    Serial.print(PWM_CHANNEL);
    Serial.println(", motorSpeed: 0 (OFF)");
  }
  Serial.print("Motor state: ");
  Serial.print(motorRunning ? "ON" : "OFF");
  Serial.print(", Speed PWM: ");
  Serial.println(motorSpeed);
  Serial.print("[DEBUG] MOTOR_IN1: ");
  Serial.print(digitalRead(MOTOR_IN1));
  Serial.print(", MOTOR_IN2: ");
  Serial.println(digitalRead(MOTOR_IN2));
}

// --- Input Handling ---

void checkEncoder() {
  static int lastPos = 0;
  static unsigned long lastChange = 0;
  const unsigned long debounceMs = 5; // 5ms debounce
  encoder.tick();
  int newPos = encoder.getPosition();

  if (newPos != lastPos) {
    unsigned long now = millis();
    if (now - lastChange > debounceMs) {
      if (newPos > lastPos) {
        motorSpeed = min(motorSpeed + 10, MAX_SPEED);
      } else {
        motorSpeed = max(motorSpeed - 10, MIN_SPEED);
      }
      lastPos = newPos;
      lastChange = now;
      updateMotor(); // Update motor speed immediately on change
      Serial.print("Encoder position: ");
      Serial.print(newPos);
      Serial.print(" | Motor speed: ");
      Serial.println(motorSpeed);
    }
  }
}

void handleEncoderClick() {
  motorRunning = !motorRunning; // Toggle motor state
  Serial.println("Encoder button clicked. Toggling motor.");
  updateMotor();
}

void handleBackClick() {
  // Increase speed
  motorSpeed = min(motorSpeed + 10, MAX_SPEED);
  Serial.println("Back button clicked. Speed increased.");
  updateMotor();
}

void handleConfirmClick() {
  // Decrease speed
  motorSpeed = max(motorSpeed - 10, MIN_SPEED);
  Serial.println("Confirm button clicked. Speed decreased.");
  updateMotor();
}

void handleLongPressConfirm() {
  // Save the current speed
  savedSpeed = motorSpeed;
  Serial.println("Long press: Speed confirmed and saved.");
  // You could also save this to EEPROM/Preferences for persistence
}