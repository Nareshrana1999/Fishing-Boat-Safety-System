# 🚤 Fishing Boat Safety System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform: Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)](https://www.arduino.cc/)

## Overview

The **Fishing Boat Safety System** is an open-source, Arduino-based solution designed to enhance the safety of fishermen at sea. It provides real-time location tracking, accident detection, and emergency alerts using a SIM800L GSM module, Neo-6M GPS, and MPU6050 accelerometer. The system monitors boat tilt, GPS coordinates, battery voltage, and manual triggers, displaying status on a 16x2 LCD and sounding a buzzer during emergencies. This project is cost-effective, easy to install, and ideal for coastal fishing communities.

---

## 🚀 Key Features
- Real-time GPS tracking (Neo-6M)
- Accident detection (MPU6050, tilt >30°)
- Emergency SMS alerts (SIM800L GSM)
- Manual emergency trigger (push button)
- Low battery monitoring and alerts
- 16x2 LCD for system status and coordinates
- Buzzer for audible emergency alerts
- Robust error handling for all modules

---

## 🛠️ Hardware Components
- **Arduino Uno** (controller)
- **Neo-6M GPS Module** (location)
- **SIM800L GSM Module** (SMS alerts)
- **MPU6050 Accelerometer + Gyroscope** (tilt detection)
- **16x2 LCD with I2C** (display)
- **Buzzer** (audible alert)
- **Push Button** (manual trigger)
- **10kΩ Resistors (x2)** (voltage divider)
- **Jumper Wires**
- **Power Supply**: 9V batteries, buck converter

---

## 🧩 System Architecture & Data Flow

The system is designed with a modular, layered architecture for reliability and scalability. Below is a box-based data flow diagram showing all components and their interactions:

```
+-------------------+      +-------------------+      +-------------------+      +-------------------+
|   MPU6050 Sensor  |      |   Neo-6M GPS      |      |   Push Button     |      | Battery Divider   |
| (Tilt/I2C: A4,A5) |      | (GPS/D4,D3)       |      | (Manual/D7)       |      | (Voltage/A0)      |
+---------+---------+      +---------+---------+      +---------+---------+      +---------+---------+
          |                          |                          |                          |
          +--------------------------+--------------------------+--------------------------+
                                         |
                                         v
                              +--------------------------+
                              |      Arduino Uno         |
                              |  (Data Processing &      |
                              |   Decision Making)       |
                              +-----------+--------------+
                                          |
              +---------------------------+---------------------------+
              |                           |                           |
              v                           v                           v
      +---------------+           +---------------+           +---------------+
      |   16x2 LCD    |           |    Buzzer     |           |   SIM800L GSM |
      | (I2C: A4,A5)  |           |   (D8)        |           | (D10, D11)    |
      +-------+-------+           +-------+-------+           +-------+-------+
              |                           |                           |
              +---------------------------+---------------------------+
                                         |
                                         v
                              +--------------------------+
                              | Emergency Contact/Rescue |
                              +--------------------------+
```

### 🧷 Component-to-Arduino Pin Mapping

| Component                | Function                | Arduino Pin(s)         | Power Source         |
|--------------------------|-------------------------|------------------------|----------------------|
| MPU6050                  | Tilt sensor (I2C)       | A4 (SDA), A5 (SCL)     | 5V, GND              |
| Neo-6M GPS Module        | GPS location (Serial)   | D4 (TX), D3 (RX)       | 5V, GND              |
| SIM800L GSM Module       | SMS alerts (Serial)     | D10 (TX), D11 (RX)     | 3.7–4.2V (Buck Conv), GND |
| 16x2 LCD with I2C        | Status display (I2C)    | A4 (SDA), A5 (SCL)     | 5V, GND              |
| Buzzer                   | Audible alert           | D8                     | GND                  |
| Push Button              | Manual trigger          | D7                     | GND                  |
| Battery Voltage Divider  | Battery monitoring      | A0                     | -                    |
| Arduino Uno              | Main controller         | -                      | 9V Battery           |

- **Note:** SIM800L GSM requires a separate, stable 3.7–4.2V supply (use a buck converter from 9V battery).
- All GNDs should be connected together for a common ground.

### 📷 Circuit Diagram

![Circuit Diagram](./Circuit%20Diagram/Circuit%20Diagram.png)

**How it works:**
- Sensors (MPU6050, GPS, Button, Battery Divider) feed data to the Arduino Uno.
- Arduino processes sensor data, detects emergencies (tilt >30°, button press, low battery), and triggers alerts.
- Alerts are displayed on the LCD, sounded via the buzzer, and sent as SMS through the GSM module.
- Emergency contacts receive real-time notifications for rapid response.

---

## ⚡ Installation and Setup

### Step 1: Gather Hardware

*   Collect all listed components.
*   Verify the SIM800L has an active 2G SIM card with sufficient balance.
*   Prepare a buck converter (e.g., adjustable step-down module) to convert 9V to 3.7–4.2V with at least 2A output.

### Step 2: Install Software

1.  Download and install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2.  Install required libraries via Sketch > Include Library > Manage Libraries:
    *   **LiquidCrystal_I2C** (Frank de Brabander).
    *   **TinyGPSPlus** (Mikal Hart).
    *   **MPU6050** (Electronic Cats or Adafruit).
    *   Confirm Wire.h and SoftwareSerial.h are built-in.
3.  Verify installation by checking the library menu.

### Step 3: Wire the Components

Connect components to Arduino Uno with exact pin-to-pin wiring:

1.  **Power Supply**:
    *   Connect a 9V battery to the Arduino Uno via the barrel jack or battery clip.
    *   Connect a second 9V battery to the input of the buck converter.
    *   Adjust the buck converter output to 3.7–4.2V (using a multimeter) and connect its positive output to the SIM800L VCC pin, negative to the SIM800L GND pin.
2.  **Neo-6M GPS Module**:
    *   **VCC** → Arduino 5V pin.
    *   **GND** → Arduino GND pin.
    *   **TX** → Arduino D4 pin.
    *   **RX** → Arduino D3 pin.
    *   Place near an open area for satellite reception.
3.  **SIM800L GSM Module**:
    *   **VCC** → Buck converter positive output (3.7–4.2V).
    *   **GND** → Buck converter negative output (connected to Arduino GND pin).
    *   **TX** → Arduino D10 pin.
    *   **RX** → Arduino D11 pin.
    *   Insert an active 2G SIM card and ensure the buck converter provides stable power.
4.  **MPU6050 Sensor**:
    *   **VCC** → Arduino 5V pin.
    *   **GND** → Arduino GND pin.
    *   **SCL** → Arduino A5 pin.
    *   **SDA** → Arduino A4 pin.
5.  **16x2 LCD with I2C Module**:
    *   **VCC** → Arduino 5V pin.
    *   **GND** → Arduino GND pin.
    *   **SCL** → Arduino A5 pin.
    *   **SDA** → Arduino A4 pin.
6.  **Buzzer**:
    *   **Positive** → Arduino D8 pin.
    *   **Negative** → Arduino GND pin.
7.  **Push Button**:
    *   **Leg 1** → Arduino D7 pin.
    *   **Leg 2** → Arduino GND pin (uses internal pull-up resistor).
8.  **Battery Voltage Divider**:
    *   **Resistor 1 (10kΩ)**: Connect one end to the 9V battery positive, other end to a junction.
    *   **Resistor 2 (10kΩ)**: Connect one end to the junction, other end to the 9V battery negative.
    *   **Junction** → Arduino A0 pin.
    *   **9V Battery Negative** → Arduino GND pin.

#### Wiring Tips

*   Use color-coded wires (red: 5V, black: GND, blue: signals).
*   Ensure the buck converter is adjusted to 3.7–4.2V with a multimeter before connecting to the SIM800L.
*   Double-check all connections to avoid shorts.

---

## 📦 Usage
- Deploy on a fishing boat.
- Monitor LCD for status and coordinates.
- Trigger emergency via tilt or button.
- Check SMS delivery to emergency contact.
- Regularly inspect batteries and SIM balance.

---

## 📜 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments
- Open-source community for libraries (TinyGPS++, MPU6050, etc.)
- Inspired by the need for affordable safety solutions in fishing communities.
