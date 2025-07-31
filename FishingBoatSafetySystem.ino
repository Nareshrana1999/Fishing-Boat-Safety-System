#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <MPU6050.h>

// Initialize modules
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27 for 16x2 LCD
SoftwareSerial gsmSerial(10, 11);   // GSM TX->Pin 10, RX->Pin 11
SoftwareSerial gpsSerial(4, 3);     // GPS TX->Pin 4, RX->Pin 3
TinyGPSPlus gps;                    // GPS data parser
MPU6050 mpu;                        // Tilt sensor

// Pin definitions
const int buzzerPin = 8;           // Buzzer for emergency alerts
const int emergencyButtonPin = 7;  // Push button for manual emergency
const int batteryPin = A0;         // Analog pin for battery voltage

// System settings
const float tiltThreshold = 30.0;  // Tilt angle threshold (degrees)
const String emergencyNumber = "+1234567890"; // Replace with actual number
const float batteryThreshold = 3.5; // Low battery threshold (volts)
const unsigned long gpsTimeout = 30000; // GPS signal timeout (ms)
const unsigned long smsInterval = 60000; // Min interval between SMS (ms)
const unsigned long debounceDelay = 50; // Debounce delay for button (ms)

// Data structure to hold sensor readings
struct SensorData {
  float latitude;
  float longitude;
  bool gpsValid;
  float tiltAngle;
  float batteryVoltage;
  bool buttonPressed;
};

// Global variables
bool emergencyState = false;        // Tracks emergency mode
unsigned long lastSMSTime = 0;     // Tracks last SMS sent time
unsigned long gpsStartTime = 0;    // Tracks GPS signal start
unsigned long lastButtonTime = 0;  // Tracks last button press for debouncing
bool lastButtonState = HIGH;       // Last button state for debouncing

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Boat Safety Sys");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MPU6050 Failed!");
    Serial.println("Error: MPU6050 connection failed");
    while (1); // Halt on failure
  }
  lcd.setCursor(0, 1);
  lcd.print("MPU6050 OK");
  delay(1000);

  // Initialize GSM
  if (!initGSM()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GSM Failed!");
    Serial.println("Error: GSM initialization failed");
    while (1); // Halt on failure
  }
  lcd.setCursor(0, 1);
  lcd.print("GSM OK");
  delay(1000);

  // Initialize pins
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  pinMode(emergencyButtonPin, INPUT_PULLUP); // Internal pull-up
  pinMode(batteryPin, INPUT);

  // Initialize GPS timer
  gpsStartTime = millis();

  // Show system ready
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(2000);
}

bool initGSM() {
  // Initialize GSM module
  gsmSerial.println("AT");
  delay(1000);
  if (gsmSerial.find("OK")) {
    gsmSerial.println("AT+CMGF=1"); // Set SMS to text mode
    delay(1000);
    if (gsmSerial.find("OK")) {
      Serial.println("GSM Initialized Successfully");
      return true;
    }
  }
  return false;
}

SensorData collectSensorData() {
  SensorData data;

  // Read GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  data.gpsValid = gps.location.isValid();
  data.latitude = data.gpsValid ? gps.location.lat() : 0.0;
  data.longitude = data.gpsValid ? gps.location.lng() : 0.0;
  Serial.print("GPS: ");
  Serial.print(data.gpsValid ? "Valid" : "Invalid");
  if (data.gpsValid) {
    Serial.print(", Lat: ");
    Serial.print(data.latitude, 6);
    Serial.print(", Lon: ");
    Serial.print(data.longitude, 6);
  }
  Serial.println();

  // Read MPU6050 data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  data.tiltAngle = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  Serial.print("Tilt Angle: ");
  Serial.print(data.tiltAngle, 1);
  Serial.println(" degrees");

  // Read battery voltage
  int sensorValue = analogRead(batteryPin);
  data.batteryVoltage = sensorValue * (5.0 / 1023.0) * 2; // Adjust for voltage divider
  Serial.print("Battery Voltage: ");
  Serial.print(data.batteryVoltage);
  Serial.println(" V");

  // Read button with debouncing
  bool currentButtonState = digitalRead(emergencyButtonPin);
  if (currentButtonState != lastButtonState) {
    lastButtonTime = millis();
  }
  if ((millis() - lastButtonTime) > debounceDelay) {
    data.buttonPressed = currentButtonState == LOW;
  } else {
    data.buttonPressed = false; // Ignore unstable state
  }
  lastButtonState = currentButtonState;
  Serial.print("Button: ");
  Serial.println(data.buttonPressed ? "Pressed" : "Released");

  return data;
}

void updateLCD(const SensorData &data) {
  lcd.clear();
  if (emergencyState) {
    lcd.setCursor(0, 0);
    lcd.print("Emergency!");
    lcd.setCursor(0, 1);
    lcd.print("Tilt:");
    lcd.print(data.tiltAngle, 1);
    lcd.print("deg");
  } else if (data.gpsValid) {
    lcd.setCursor(0, 0);
    lcd.print("Lat:");
    lcd.print(data.latitude, 4);
    lcd.setCursor(0, 1);
    lcd.print("Lon:");
    lcd.print(data.longitude, 4);
  } else if (millis() - gpsStartTime > gpsTimeout) {
    lcd.setCursor(0, 0);
    lcd.print("No GPS Signal");
    lcd.setCursor(0, 1);
    lcd.print("Check GPS");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Searching GPS...");
  }

  // Show battery status
  if (data.batteryVoltage < batteryThreshold) {
    lcd.setCursor(12, 1);
    lcd.print("LOW");
  }
}

void sendEmergencySMS(float lat, float lon) {
  String smsMessage = "Emergency! Boat Accident. Coordinates: Lat " + String(lat, 6) + ", Lon " + String(lon, 6);
  Serial.println("Sending SMS: " + smsMessage);
  gsmSerial.println("AT+CMGS=\"" + emergencyNumber + "\"");
  delay(100);
  gsmSerial.print(smsMessage);
  delay(100);
  gsmSerial.write(26); // Send Ctrl+Z to indicate end of message
  delay(1000);
  if (gsmSerial.find("OK")) {
    Serial.println("SMS Sent Successfully");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emergency!");
    lcd.setCursor(0, 1);
    lcd.print("SMS Sent");
    delay(2000);
  } else {
    Serial.println("Error: SMS Failed");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emergency!");
    lcd.setCursor(0, 1);
    lcd.print("SMS Failed");
    delay(2000);
  }
}

void loop() {
  // Collect all sensor data
  SensorData data = collectSensorData();

  // Update LCD
  updateLCD(data);

  // Check for emergency conditions
  if ((abs(data.tiltAngle) > tiltThreshold || data.buttonPressed) && !emergencyState) {
    emergencyState = true;
    digitalWrite(buzzerPin, HIGH); // Activate buzzer
    if (data.gpsValid && (millis() - lastSMSTime) > smsInterval) {
      sendEmergencySMS(data.latitude, data.longitude);
      lastSMSTime = millis();
    } else {
      Serial.println("No GPS for SMS");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Emergency!");
      lcd.setCursor(0, 1);
      lcd.print("No GPS for SMS");
      delay(2000);
    }
  } else if (emergencyState && data.buttonPressed) {
    // Reset emergency on second button press
    emergencyState = false;
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emergency Reset");
    Serial.println("Emergency Reset");
    delay(2000);
  }

  delay(1000); // Update every second
}