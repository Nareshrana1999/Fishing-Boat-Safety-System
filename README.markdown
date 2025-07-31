# Fishing Boat Safety System

## Overview

The **Fishing Boat Safety System** is an open-source Arduino-based project developed by [Nareshrana1999](https://github.com/Nareshrana1999) to enhance the safety of fishermen at sea. Hosted at [https://github.com/Nareshrana1999/Fishing-Boat-Safety-System](https://github.com/Nareshrana1999/Fishing-Boat-Safety-System), this project provides real-time location tracking, accident detection, and emergency alerts using a single SIM800L GSM module, a Neo-6M GPS module, and an MPU6050 accelerometer. The system monitors boat tilt, GPS coordinates, battery voltage, and manual triggers, displaying status on a 16x2 LCD and sounding a buzzer during emergencies. This project offers a cost-effective and easy-to-install solution for coastal fishing communities.

### Key Features
- Real-time GPS tracking with the Neo-6M module.
- Accident detection using MPU6050 (tilt >30°).
- Emergency SMS alerts via a single SIM800L GSM module.
- Manual emergency trigger via a push button.
- Low battery monitoring (alerts below 3.5V).
- 16x2 LCD display for system status and coordinates.
- Buzzer for audible emergency alerts.
- Robust error handling for GPS, GSM, and sensor failures.

### Applications
- Safety monitoring for fishing boats, yachts, and water taxis.
- Disaster management and rescue operations.
- Real-time tracking for families and coast guards.

### Limitations
- Relies on 2G network coverage, which may be unavailable in deep sea.
- GPS accuracy depends on weather and satellite visibility.
- Single GSM module limits redundancy.

### Future Scope
- Integrate a backup communication module (e.g., LoRa).
- Develop a mobile app for live tracking.
- Add a solar power module for self-charging.
- Include a camera or voice alert system.

## Project Details

### Hardware Components
- **Arduino Uno**: Microcontroller for processing and control.
- **Neo-6M GPS Module**: Provides real-time location data.
- **SIM800L GSM Module**: Sends emergency SMS (requires an active 2G SIM card).
- **MPU6050 Accelerometer + Gyroscope**: Detects boat tilt.
- **16x2 LCD with I2C Module**: Displays system status and coordinates.
- **Buzzer**: Sounds during emergencies.
- **Push Button**: Manual emergency trigger.
- **10kΩ Resistors (x2)**: For battery voltage divider.
- **Jumper Wires**: For connections.
- **Power Supply**:
  - 9V battery for Arduino (via barrel jack or battery clip).
  - Buck converter (e.g., adjustable step-down module, 9V to 3.7–4.2V, 2A capacity) with 9V battery for SIM800L.
  - 9V battery for the voltage divider circuit.

### Software Requirements
- **Arduino IDE**: For programming and uploading code (download from [arduino.cc](https://www.arduino.cc/en/software)).
- **Libraries**:
  - **Wire.h**: Built-in, for I2C communication.
  - **LiquidCrystal_I2C.h**: For LCD control (by Frank de Brabander).
  - **SoftwareSerial.h**: Built-in, for GPS and GSM serial communication.
  - **TinyGPS++.h**: For GPS data parsing (by Mikal Hart).
  - **MPU6050.h**: For tilt sensor (by Electronic Cats or Adafruit).
  - Installation: Use `Sketch > Include Library > Manage Libraries` in Arduino IDE.

### System Architecture
The system collects data from sensors, processes it with Arduino Uno, and outputs alerts via LCD, buzzer, and a single GSM module. The architecture includes:
- **Inputs**: MPU6050 (tilt via I2C: A4, A5), Neo-6M GPS (serial: D4, D3), push button (D7), battery voltage (A0).
- **Processing**: Arduino evaluates data and triggers alerts based on tilt (>30°) or button press.
- **Outputs**: LCD (I2C: A4, A5), buzzer (D8), GSM (serial: D10, D11).

```plaintext
[MPU6050 Sensor]       [Neo-6M GPS]       [Push Button]       [Battery Divider]
       ↓                    ↓                   ↓                  ↓
   [Tilt Data]        [Coordinates]       [Manual Trigger]    [Voltage]
   (I2C: A4, A5)     (Serial: D4, D3)     (Digital: D7)      (Analog: A0)
       ↓                    ↓                   ↓                  ↓
       →→→→→→→→→→→→→ [Arduino Uno] ←←←←←←←←←←←←←←←←←←←←←←←←←←
                 ↓
     ┌───────────────┬──────────────┐
     ↓               ↓              ↓
 [16x2 LCD]       [Buzzer]     [SIM800L GSM]
 (I2C: A4, A5)    (D8)         (Serial: D10, D11)
                                  ↓
                          [Emergency Contact]
```

## Installation and Setup

### Step 1: Gather Hardware
- Collect all listed components.
- Verify the SIM800L has an active 2G SIM card with sufficient balance.
- Prepare a buck converter (e.g., adjustable step-down module) to convert 9V to 3.7–4.2V with at least 2A output.

### Step 2: Install Software
1. Download and install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2. Install required libraries via `Sketch > Include Library > Manage Libraries`:
   - **LiquidCrystal_I2C** (Frank de Brabander).
   - **TinyGPSPlus** (Mikal Hart).
   - **MPU6050** (Electronic Cats or Adafruit).
   - Confirm `Wire.h` and `SoftwareSerial.h` are built-in.
3. Verify installation by checking the library menu.

### Step 3: Wire the Components
Connect components to Arduino Uno with exact pin-to-pin wiring:

1. **Power Supply**:
   - Connect a 9V battery to the Arduino Uno via the barrel jack or battery clip.
   - Connect a second 9V battery to the input of the buck converter.
   - Adjust the buck converter output to 3.7–4.2V (using a multimeter) and connect its positive output to the SIM800L VCC pin, negative to the SIM800L GND pin.

2. **Neo-6M GPS Module**:
   - **VCC** → Arduino 5V pin.
   - **GND** → Arduino GND pin.
   - **TX** → Arduino D4 pin.
   - **RX** → Arduino D3 pin.
   - Place near an open area for satellite reception.

3. **SIM800L GSM Module**:
   - **VCC** → Buck converter positive output (3.7–4.2V).
   - **GND** → Buck converter negative output (connected to Arduino GND pin).
   - **TX** → Arduino D10 pin.
   - **RX** → Arduino D11 pin.
   - Insert an active 2G SIM card and ensure the buck converter provides stable power.

4. **MPU6050 Sensor**:
   - **VCC** → Arduino 5V pin.
   - **GND** → Arduino GND pin.
   - **SCL** → Arduino A5 pin.
   - **SDA** → Arduino A4 pin.

5. **16x2 LCD with I2C Module**:
   - **VCC** → Arduino 5V pin.
   - **GND** → Arduino GND pin.
   - **SCL** → Arduino A5 pin.
   - **SDA** → Arduino A4 pin.

6. **Buzzer**:
   - **Positive** → Arduino D8 pin.
   - **Negative** → Arduino GND pin.

7. **Push Button**:
   - **Leg 1** → Arduino D7 pin.
   - **Leg 2** → Arduino GND pin (uses internal pull-up resistor).

8. **Battery Voltage Divider**:
   - **Resistor 1 (10kΩ)**: Connect one end to the 9V battery positive, other end to a junction.
   - **Resistor 2 (10kΩ)**: Connect one end to the junction, other end to the 9V battery negative.
   - **Junction** → Arduino A0 pin.
   - **9V Battery Negative** → Arduino GND pin.

#### Wiring Tips
- Use color-coded wires (red: 5V, black: GND, blue: signals).
- Ensure the buck converter is adjusted to 3.7–4.2V with a multimeter before connecting to the SIM800L.
- Double-check all connections to avoid shorts.

### Step 4: Upload the Code
1. Clone this repository (`git clone https://github.com/Nareshrana1999/Fishing-Boat-Safety-System.git`) or download the `FishingBoatSafetySystem.ino` file.
2. Open the sketch in Arduino IDE.
3. Update the `emergencyNumber` with a valid phone number (e.g., `+1234567890`).
4. Connect Arduino Uno via USB, select `Tools > Board > Arduino Uno`, choose the port, and upload.

### Step 5: Test the System
1. **Power On**:
   - Arduino: 9V battery via barrel jack or clip.
   - SIM800L: 9V battery through the buck converter (adjusted to 3.7–4.2V).
   - Ensure network coverage.
2. **Verify LCD**:
   - Displays “Boat Safety Sys”, “Initializing…”, “MPU6050 OK”, “GSM OK”, then “System Ready”.
   - Normal: Shows coordinates or “Searching GPS…”.
   - Emergency: Shows “Emergency!” and tilt angle.
   - Low battery: Shows “LOW”.
3. **Test Features**:
   - **Tilt**: Tilt MPU6050 > 30° (buzzer on, SMS sent).
   - **Button**: Press to trigger/reset emergency.
   - **GPS**: Test outdoors (1–2 min lock).
   - **SMS**: Verify SMS at the emergency number.
   - **Battery**: Check “LOW” if <3.5V.
4. **Debugging**:
   - Use Serial Monitor (`Tools > Serial Monitor`, 9600 baud) to check GPS, tilt, battery, button, and SMS status.

### Step 6: Final Assembly
- Secure components with jumper wires or transfer to a PCB.
- Position the GPS module in an open area.
- Ensure the SIM800L has network coverage (test with `AT+CSQ`).
- Use a waterproof, marine-grade enclosure.
- Calibrate the `tiltThreshold` (30°) and `batteryThreshold` (3.5V).
- Test on a dock before sea deployment.

## Usage
- Deploy the system on a fishing boat.
- Monitor the LCD for status and coordinates.
- Press the button or tilt the boat to trigger an emergency.
- Check SMS delivery to the emergency contact.
- Regularly inspect the 9V batteries and SIM balance.

## Circuit Diagram
See the circuit preview for detailed wiring: [https://app.cirkitdesigner.com/project/29163fd1-7f0c-47da-bc2a-d4b686db9c3b](https://app.cirkitdesigner.com/project/29163fd1-7f0c-47da-bc2a-d4b686db9c3b).

## Contributing
We welcome contributions to improve this project! To contribute:
1. Fork this repository (`https://github.com/Nareshrana1999/Fishing-Boat-Safety-System`).
2. Create a branch (`git checkout -b feature/YourFeature`).
3. Commit changes (`git commit -m "Add YourFeature"`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.
- Report issues or suggest features via the [Issues](https://github.com/Nareshrana1999/Fishing-Boat-Safety-System/issues) tab.
- Follow the code style in `FishingBoatSafetySystem.ino`.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Thanks to the open-source community for libraries like TinyGPS++ and MPU6050.
- Inspired by the need for affordable safety solutions in fishing communities.