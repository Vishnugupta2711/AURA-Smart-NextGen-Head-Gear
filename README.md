# AURA - nextGEN Smart Helmet
![aura](https://github.com/user-attachments/assets/d91c1ca6-9aab-442d-827b-c5ed2c106f98)

AURA is an innovative IoT-based smart helmet designed to enhance rider safety through features like crash detection, a smart locking system, real-time accident alerts, and an alcohol detection sensor. Built with the latest IoT technology, AURA aims to make roads safer for riders.

---

## 🚀 Features

- **Crash Detection**  
  Instantly detects accidents and sends the exact location to emergency services to ensure timely assistance.

- **Smart Locking System**  
  Identifies if the helmet is worn. The ignition starts only when the helmet is worn, and remains off if the helmet is not worn.

- **Real-Time Accident Alerts & Emergency Notifications**  
  In case of a collision or impact, the helmet sends an alert with the rider's location to emergency contacts or nearby hospitals.

- **Alcohol Detection**  
  The built-in alcohol sensor acts as a breath analyzer, preventing ignition if the alcohol level exceeds the permissible range.

---

## 📐 Project Architecture

- **Microcontroller**: Arduino (e.g., Arduino Uno)
- **Sensors & Modules**:
  - **Accelerometer/Gyroscope**: MPU6050 for crash detection.
  - **ESP8266 Module**: WiFi connection to transmit data to a web server or cloud service.
  - **Alcohol Sensor**: MQ-3 for breath analysis.
  - **Relay Module**: Controls ignition system based on helmet status.
  - **Pressure Sensor**: Detects if the helmet is worn or not.
  
---

## ⚙️ Setup & Installation

1. **Hardware Connections**:  
   - Connect the MPU6050 sensor for impact detection and communicate location and notifications.
   - Attach the MQ-3 alcohol sensor near the helmet’s mouth area.
   - Set up the pressure sensor to detect helmet usage.
   - Configure the ESP8266 for WiFi connectivity to enable online notifications and cloud data logging.

2. **Software Installation**:
   - Clone this repository.
   - Upload the Arduino code (`AURA.ino`) to the Arduino board.
   - Configure the ESP8266 modules by following the setup instructions.

---

## 🚨 Usage

1. **Starting the Helmet**: Wear the helmet to activate the smart lock system, enabling the vehicle’s ignition.
2. **Crash Detection**: In case of an accident, the helmet detects the impact and immediately sends an SMS alert with the location coordinates and logs the data online.
3. **Alcohol Detection**: Before ignition, AURA checks the alcohol levels. If they exceed the set threshold, ignition is disabled, ensuring safe driving.

---



**AURA** - *Empowering Safety, Enabling Innovation.*
