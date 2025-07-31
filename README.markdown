# 🚤 Fishing Boat Safety System

## 📝 Overview

The **Fishing Boat Safety System** is an open-source, Arduino-based solution designed to enhance the safety of fishermen at sea. It provides real-time location tracking, accident detection, and emergency alerts using a SIM800L GSM module, Neo-6M GPS, and MPU6050 accelerometer. The system monitors boat tilt, GPS coordinates, battery voltage, and manual triggers, displaying status on a 16x2 LCD and sounding a buzzer during emergencies. This project is cost-effective, easy to install, and ideal for coastal fishing communities.

---

## ✨ Key Features

- 🛰️ Real-time GPS tracking (Neo-6M)
- ⚠️ Accident detection (MPU6050, tilt >30°)
- 📲 Emergency SMS alerts (SIM800L GSM)
- 🆘 Manual emergency trigger (push button)
- 🔋 Low battery monitoring and alerts
- 📟 16x2 LCD for system status and coordinates
- 🔊 Buzzer for audible emergency alerts
- 🛡️ Robust error handling for all modules

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

<p align="center">
  <img src="./Circuit%20Diagram/Circuit%20Diagram.png" alt="Circuit Diagram" width="900"/>
</p>

---

## 🗂️ Component-to-Arduino Pin Mapping

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

> **Note:** SIM800L GSM requires a separate, stable 3.7–4.2V supply (use a buck converter from 9V battery). All GNDs should be connected together for a common ground.

---

## ⚡ Installation & Quick Start

### 1️⃣ Step 1: Gather Hardware
- Collect all listed components.
- Verify the SIM800L has an active 2G SIM card with sufficient balance.
- Prepare a buck converter (e.g., adjustable step-down module) to convert 9V to 3.7–4.2V with at least 2A output.

### 2️⃣ Step 2: Install Software
1. Download and install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2. Install required libraries via Sketch > Include Library > Manage Libraries:
   - **LiquidCrystal_I2C** (Frank de Brabander)
   - **TinyGPSPlus** (Mikal Hart)
   - **MPU6050** (Electronic Cats or Adafruit)
   - Confirm Wire.h and SoftwareSerial.h are built-in
3. Verify installation by checking the library menu.

---

## 🪡 Wiring Instructions

Connect components to Arduino Uno with exact pin-to-pin wiring:

### 🔌 Power Supply
- Connect a 9V battery to the Arduino Uno via the barrel jack or battery clip.
- Connect a second 9V battery to the input of the buck converter.
- Adjust the buck converter output to 3.7–4.2V (using a multimeter) and connect its positive output to the SIM800L VCC pin, negative to the SIM800L GND pin.

### 🛰️ Neo-6M GPS Module
- **VCC** → Arduino 5V pin
- **GND** → Arduino GND pin
- **TX** → Arduino D4 pin
- **RX** → Arduino D3 pin
- Place near an open area for satellite reception.

### 📲 SIM800L GSM Module
- **VCC** → Buck converter positive output (3.7–4.2V)
- **GND** → Buck converter negative output (connected to Arduino GND pin)
- **TX** → Arduino D10 pin
- **RX** → Arduino D11 pin
- Insert an active 2G SIM card and ensure the buck converter provides stable power.

### 🧭 MPU6050 Sensor
- **VCC** → Arduino 5V pin
- **GND** → Arduino GND pin
- **SCL** → Arduino A5 pin
- **SDA** → Arduino A4 pin

### 📟 16x2 LCD with I2C Module
- **VCC** → Arduino 5V pin
- **GND** → Arduino GND pin
- **SCL** → Arduino A5 pin
- **SDA** → Arduino A4 pin

### 🔊 Buzzer
- **Positive** → Arduino D8 pin
- **Negative** → Arduino GND pin

### 🆘 Push Button
- **Leg 1** → Arduino D7 pin
- **Leg 2** → Arduino GND pin (uses internal pull-up resistor)

### 🔋 Battery Voltage Divider
- **Resistor 1 (10kΩ)**: Connect one end to the 9V battery positive, other end to a junction.
- **Resistor 2 (10kΩ)**: Connect one end to the junction, other end to the 9V battery negative.
- **Junction** → Arduino A0 pin
- **9V Battery Negative** → Arduino GND pin

#### 💡 Wiring Tips
- Use color-coded wires (red: 5V, black: GND, blue: signals).
- Ensure the buck converter is adjusted to 3.7–4.2V with a multimeter before connecting to the SIM800L.
- Double-check all connections to avoid shorts.

---

## 📦 Usage
- Deploy on a fishing boat.
- Monitor LCD for status and coordinates.
- Trigger emergency via tilt or button.
- Check SMS delivery to emergency contact.
- Regularly inspect batteries and SIM balance.

---

## 🖥️ Detailed Circuit Online

<p align="center">
  <a href="https://app.cirkitdesigner.com/project/29163fd1-7f0c-47da-bc2a-d4b686db9c3b" target="_blank">
    <img src="https://app.cirkitdesigner.com/favicon.ico" width="40" alt="Cirkit Designer Logo"/>
  </a>
</p>

<p align="center">
  View the full interactive circuit and all detailed connections on Cirkit Designer:<br>
  <a href="https://app.cirkitdesigner.com/project/29163fd1-7f0c-47da-bc2a-d4b686db9c3b"><strong>Open Circuit Project</strong></a>
</p>

---

## 📜 License

<div align="center">
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="MIT License" style="vertical-align:middle; margin-right:10px;"/>
</div>

<p align="center" style="font-size:1.1em;">
  <strong>This project is licensed under the <a href="LICENSE">MIT License</a>.</strong><br>
  You are free to use, modify, and distribute this software with proper attribution.<br>
  See the <a href="LICENSE">LICENSE</a> file for full details.
</p>

---

## 🙏 Acknowledgments
- Open-source community for libraries (TinyGPS++, MPU6050, etc.)
- Inspired by the need for affordable safety solutions in fishing communities.
