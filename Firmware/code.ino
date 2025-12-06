/*
 * ESP32-S3 Modular Robot Base Firmware
 * 
 * Hardware:
 * - ESP32-S3 WROOM module
 * - DRV8833 dual motor driver
 * - Analog joystick (X, Y, SW)
 * - Headlight module (LED control via GPIO)
 * - 3 buttons: RESET, BOOT, USER
 * - Status LED
 * 
 * Controls:
 * - Joystick X/Y: Tank-style motor control
 * - Joystick button press: Toggle headlights
 * - User button: Emergency motor stop
 */

// ===== PIN DEFINITIONS =====

// Motor Driver (DRV8833)
#define MOTOR_AIN1  6   // Left motor direction 1
#define MOTOR_AIN2  7   // Left motor direction 2
#define MOTOR_BIN1  15  // Right motor direction 1
#define MOTOR_BIN2  16  // Right motor direction 2

// PWM channels for motor control
#define PWM_FREQ    20000  // 20kHz PWM frequency
#define PWM_RES     8      // 8-bit resolution (0-255)
#define PWM_CH_A1   0
#define PWM_CH_A2   1
#define PWM_CH_B1   2
#define PWM_CH_B2   3

// Headlight Module (Pogo Pin)
#define LED_CTRL    5   // Headlight control GPIO
#define EXTRA_GPIO  4   // Extra GPIO (unused)

// Joystick (analog + button)
#define JOY_X       1   // ADC pin for X-axis (GPIO1)
#define JOY_Y       2   // ADC pin for Y-axis (GPIO2)
#define JOY_SW      21  // Joystick button (shared with USER button)

// Buttons
#define BTN_USER    21  // User button / Joystick SW
#define BTN_BOOT    0   // Boot button (GPIO0)

// Status LED
#define STATUS_LED  48  // Status indicator LED

// ===== GLOBAL VARIABLES =====

bool headlightsOn = false;
bool motorsEnabled = true;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200; // ms

// Joystick center calibration
int joyXCenter = 2048;  // ADC center value (12-bit: 0-4095)
int joyYCenter = 2048;
const int deadzone = 300; // Joystick deadzone

// ===== SETUP =====

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== ESP32-S3 Robot Starting ===");
  
  // Configure motor pins with PWM
  setupMotors();
  
  // Configure GPIO pins
  pinMode(LED_CTRL, OUTPUT);
  pinMode(EXTRA_GPIO, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BTN_USER, INPUT_PULLUP);
  pinMode(BTN_BOOT, INPUT_PULLUP);
  
  // Configure ADC for joystick
  analogReadResolution(12); // 12-bit ADC (0-4095)
  analogSetAttenuation(ADC_11db); // Full range 0-3.3V
  
  // Initial states
  digitalWrite(LED_CTRL, LOW);
  digitalWrite(STATUS_LED, HIGH); // Power on indicator
  
  // Calibrate joystick center
  calibrateJoystick();
  
  // Startup blink
  for(int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED, LOW);
    delay(100);
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
  }
  
  Serial.println("Robot initialized!");
  Serial.println("Joystick controls motors, press button for headlights");
}

// ===== MAIN LOOP =====

void loop() {
  // Read joystick position
  int joyX = analogRead(JOY_X);
  int joyY = analogRead(JOY_Y);
  
  // Read buttons
  bool btnPressed = (digitalRead(BTN_USER) == LOW);
  bool bootPressed = (digitalRead(BTN_BOOT) == LOW);
  
  // Handle button press (toggle headlights)
  if(btnPressed && (millis() - lastButtonPress > debounceDelay)) {
    toggleHeadlights();
    lastButtonPress = millis();
  }
  
  // BOOT button = emergency stop
  if(bootPressed) {
    motorsEnabled = false;
    stopMotors();
    blinkStatusLED();
  } else {
    motorsEnabled = true;
  }
  
  // Process joystick and control motors
  if(motorsEnabled) {
    controlMotors(joyX, joyY);
  }
  
  // Status LED heartbeat (slow blink when running)
  static unsigned long lastBlink = 0;
  if(millis() - lastBlink > 1000) {
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    lastBlink = millis();
  }
  
  delay(20); // 50Hz control loop
}

// ===== MOTOR CONTROL FUNCTIONS =====

void setupMotors() {
  // Configure PWM channels for each motor pin
  ledcSetup(PWM_CH_A1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_A2, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_B1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_B2, PWM_FREQ, PWM_RES);
  
  // Attach pins to PWM channels
  ledcAttachPin(MOTOR_AIN1, PWM_CH_A1);
  ledcAttachPin(MOTOR_AIN2, PWM_CH_A2);
  ledcAttachPin(MOTOR_BIN1, PWM_CH_B1);
  ledcAttachPin(MOTOR_BIN2, PWM_CH_B2);
  
  stopMotors();
}

void controlMotors(int joyX, int joyY) {
  // Convert joystick to motor speeds (tank drive)
  // Y-axis = forward/backward
  // X-axis = left/right turning
  
  int forward = map(joyY, 0, 4095, -255, 255);
  int turn = map(joyX, 0, 4095, -255, 255);
  
  // Apply center offset
  forward = forward - map(joyYCenter, 0, 4095, -255, 255);
  turn = turn - map(joyXCenter, 0, 4095, -255, 255);
  
  // Apply deadzone
  if(abs(forward) < map(deadzone, 0, 4095, 0, 255)) forward = 0;
  if(abs(turn) < map(deadzone, 0, 4095, 0, 255)) turn = 0;
  
  // Calculate left and right motor speeds (tank drive mixing)
  int leftSpeed = constrain(forward + turn, -255, 255);
  int rightSpeed = constrain(forward - turn, -255, 255);
  
  // Drive motors
  setMotorA(leftSpeed);
  setMotorB(rightSpeed);
  
  // Debug output (optional - comment out for performance)
  static unsigned long lastPrint = 0;
  if(millis() - lastPrint > 500) {
    Serial.printf("Joy X:%d Y:%d | Fwd:%d Turn:%d | L:%d R:%d\n", 
                  joyX, joyY, forward, turn, leftSpeed, rightSpeed);
    lastPrint = millis();
  }
}

void setMotorA(int speed) {
  // Motor A (left) control
  if(speed > 0) {
    // Forward
    ledcWrite(PWM_CH_A1, speed);
    ledcWrite(PWM_CH_A2, 0);
  } else if(speed < 0) {
    // Reverse
    ledcWrite(PWM_CH_A1, 0);
    ledcWrite(PWM_CH_A2, -speed);
  } else {
    // Brake
    ledcWrite(PWM_CH_A1, 0);
    ledcWrite(PWM_CH_A2, 0);
  }
}

void setMotorB(int speed) {
  // Motor B (right) control
  if(speed > 0) {
    // Forward
    ledcWrite(PWM_CH_B1, speed);
    ledcWrite(PWM_CH_B2, 0);
  } else if(speed < 0) {
    // Reverse
    ledcWrite(PWM_CH_B1, 0);
    ledcWrite(PWM_CH_B2, -speed);
  } else {
    // Brake
    ledcWrite(PWM_CH_B1, 0);
    ledcWrite(PWM_CH_B2, 0);
  }
}

void stopMotors() {
  ledcWrite(PWM_CH_A1, 0);
  ledcWrite(PWM_CH_A2, 0);
  ledcWrite(PWM_CH_B1, 0);
  ledcWrite(PWM_CH_B2, 0);
}

// ===== HEADLIGHT CONTROL =====

void toggleHeadlights() {
  headlightsOn = !headlightsOn;
  digitalWrite(LED_CTRL, headlightsOn ? HIGH : LOW);
  
  Serial.print("Headlights: ");
  Serial.println(headlightsOn ? "ON" : "OFF");
  
  // Quick status LED flash to confirm
  digitalWrite(STATUS_LED, LOW);
  delay(50);
  digitalWrite(STATUS_LED, HIGH);
}

// ===== UTILITY FUNCTIONS =====

void calibrateJoystick() {
  Serial.println("Calibrating joystick center...");
  delay(500);
  
  // Sample center position multiple times
  long sumX = 0, sumY = 0;
  for(int i = 0; i < 10; i++) {
    sumX += analogRead(JOY_X);
    sumY += analogRead(JOY_Y);
    delay(10);
  }
  
  joyXCenter = sumX / 10;
  joyYCenter = sumY / 10;
  
  Serial.printf("Joystick center: X=%d, Y=%d\n", joyXCenter, joyYCenter);
}

void blinkStatusLED() {
  // Fast blink for error/stop
  for(int i = 0; i < 5; i++) {
    digitalWrite(STATUS_LED, LOW);
    delay(50);
    digitalWrite(STATUS_LED, HIGH);
    delay(50);
  }
}

// ===== OPTIONAL: Battery Monitoring (if you add voltage divider) =====
/*
#define VBAT_PIN 3  // ADC pin for battery voltage
#define VBAT_DIVIDER 2.0  // Voltage divider ratio

float readBatteryVoltage() {
  int raw = analogRead(VBAT_PIN);
  float voltage = (raw / 4095.0) * 3.3 * VBAT_DIVIDER;
  return voltage;
}

void checkBattery() {
  float vbat = readBatteryVoltage();
  if(vbat < 3.3) {
    // Low battery warning
    Serial.println("WARNING: Low battery!");
    // Blink LED rapidly
  }
}
*/