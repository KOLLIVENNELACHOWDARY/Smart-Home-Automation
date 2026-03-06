# 🏠 Smart Home Automation System with IoT & AI

A comprehensive three-tier smart home automation system combining Arduino-based local control, ESP32 IoT connectivity, and ESP32-CAM AI-powered face recognition for secure access control.

![System Architecture](docs/images/system-architecture.png)

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Wiring Diagrams](#wiring-diagrams)
- [Usage](#usage)
- [Code Structure](#code-structure)
---

## 🎯 Overview

This project implements a fully automated smart home system with:

- **Local Automation**: Arduino Uno controls irrigation, lighting, parking gate, and rain cover
- **IoT Integration**: ESP32 connects to Firebase and Blynk for cloud monitoring and control
- **AI Security**: ESP32-CAM provides face recognition-based door access control
- **Real-time Monitoring**: Temperature, humidity, smoke detection, and motion sensing
- **Mobile Control**: Blynk app for remote monitoring and notifications

---

## ✨ Features

### 🌱 Automatic Irrigation System
- Soil moisture monitoring
- Rain detection to prevent watering during rainfall
- Automatic water pump control
- Water conservation

### 💡 Smart Lighting
- Motion-activated room lights
- IR sensor-based detection
- Automatic on/off control

### 🚗 Automatic Parking Gate
- Ultrasonic distance sensor
- Auto-open when vehicle approaches (< 20cm)
- Auto-close when vehicle moves away
- Servo-controlled gate mechanism

### 🌧 Rain Protection Cover
- Automatic rain detection
- Servo-controlled protective cover
- Prevents damage to outdoor equipment

### 🌡️ Environmental Monitoring
- Real-time temperature and humidity tracking (DHT11)
- Smoke/gas detection (MQ-2/MQ-135)
- Automatic alerts for dangerous conditions
- Cloud data logging

### 🔐 AI-Powered Face Recognition Door Lock
- Neural network-based face detection
- Multi-face enrollment capability
- Web interface for face management
- Automatic door unlock for authorized users
- Security logging

### 📱 Mobile App Integration
- Real-time sensor data visualization
- Push notifications for alerts
- Remote monitoring via Blynk
- Historical data graphs

### ☁️ Cloud Integration
- Firebase Realtime Database
- Data synchronization
- Remote access
- Event logging

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    SMART HOME ECOSYSTEM                          │
└─────────────────────────────────────────────────────────────────┘
                              │
                ┌─────────────┴─────────────┐
                │                           │
                ▼                           ▼
    ┌───────────────────┐       ┌──────────────────┐
    │   Arduino Uno     │       │   ESP32-CAM      │
    │ (Local Control)   │       │ (Face Recognition)│
    └───────────────────┘       └──────────────────┘
            │                            │
            │                            │
    ┌───────┴────────┐          ┌────────┴─────────┐
    │                │          │                  │
    ▼                ▼          ▼                  ▼
Sensors:         Actuators:  Camera            Servo
• Soil           • Pump      OV2640            Door Lock
• Rain           • Light
• Ultrasonic     • Servos
• IR Motion
                    │
                    │ Serial (9600 baud)
                    ▼
            ┌──────────────┐
            │   ESP32      │
            │  (IoT Hub)   │
            └──────────────┘
                    │
            ┌───────┴────────┐
            │                │
            ▼                ▼
        Sensors:         Trigger:
        • DHT11          • IR Motion
        • MQ Smoke       • ESP32-CAM
        • IR Motion
                    │
                    │ WiFi
                    ▼
        ┌───────────────────────┐
        │   Cloud Services      │
        ├───────────────────────┤
        │ • Firebase Database   │
        │ • Blynk IoT Platform  │
        │ • Mobile App          │
        └───────────────────────┘
```

---

## 🛠️ Hardware Requirements

### Module 1: Arduino Uno (Local Automation)

| Component | Quantity | Specification | Purpose |
|-----------|----------|---------------|---------|
| Arduino Uno | 1 | ATmega328P | Main controller |
| Soil Moisture Sensor | 1 | Analog output | Irrigation control |
| Rain Sensor | 1 | Digital output | Rain detection |
| IR Motion Sensor | 1 | PIR/IR module | Room motion detection |
| Ultrasonic Sensor | 1 | HC-SR04 | Parking distance measurement |
| Servo Motor | 2 | SG90 (or similar) | Gate & rain cover |
| Water Pump | 1 | 5V/12V DC | Irrigation |
| Relay Module | 1 | 3-channel, 5V | Pump & light control |
| Light Bulb | 1 | AC/DC compatible | Room lighting |
| Jumper Wires | 40+ | Male-to-male/female | Connections |
| Breadboard | 1 | 830 point | Prototyping |
| Power Supply | 1 | 9-12V, 1A | Arduino power |

### Module 2: ESP32 (IoT Gateway)

| Component | Quantity | Specification | Purpose |
|-----------|----------|---------------|---------|
| ESP32 DevKit | 1 | ESP32-WROOM-32 | IoT controller |
| DHT11 Sensor | 1 | Temperature & humidity | Environmental monitoring |
| MQ-2/MQ-135 | 1 | Smoke/gas sensor | Safety monitoring |
| IR Motion Sensor | 1 | PIR module | Security detection |
| Jumper Wires | 20+ | Male-to-male/female | Connections |
| Breadboard | 1 | 400 point | Prototyping |
| Power Supply | 1 | 5V, 2A (USB) | ESP32 power |

### Module 3: ESP32-CAM (Face Recognition)

| Component | Quantity | Specification | Purpose |
|-----------|----------|---------------|---------|
| ESP32-CAM | 1 | AI-Thinker model | Face recognition |
| OV2640 Camera | 1 | 2MP (included) | Image capture |
| Servo Motor | 1 | SG90 | Door lock mechanism |
| FTDI Programmer | 1 | USB-to-TTL, 5V | Code upload |
| Power Supply | 1 | 5V, 2A | Stable power |
| Jumper Wires | 10+ | Male-to-female | Connections |

### Additional Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| Power Strips | 2-3 | For multiple power supplies |
| USB Cables | 3 | For programming & power |
| Enclosures | 3 | Protect modules from dust/water |
| Mounting Hardware | As needed | Screws, standoffs, brackets |

---

## 💻 Software Requirements

### Development Environment

| Software | Version | Purpose |
|----------|---------|---------|
| Arduino IDE | 1.8.x or 2.x | Code development & upload |
| ESP32 Board Package | Latest | ESP32 support in Arduino IDE |
| Git | Latest | Version control |

### Arduino Libraries

Install via Arduino IDE Library Manager:

```
Servo               (Built-in)
```

### ESP32 Libraries

```
WiFi                (Built-in with ESP32 package)
FirebaseESP32       by Mobizt (v4.x.x)
Blynk               by Volodymyr Shymanskyy (v1.x.x)
DHT sensor library  by Adafruit (v1.4.x)
Adafruit Unified Sensor (dependency)
ESP32Servo          by Kevin Harrington
```

### ESP32-CAM Libraries

```
esp_camera          (Built-in with ESP32 package)
WiFi                (Built-in)
ESP32Servo          by Kevin Harrington
```

### Cloud Services Accounts

1. **Firebase**: [console.firebase.google.com](https://console.firebase.google.com)
   - Create project
   - Enable Realtime Database
   - Get Database URL and Secret Key

2. **Blynk**: [blynk.io](https://blynk.io)
   - Download mobile app (iOS/Android)
   - Create new project
   - Get Auth Token

3. **WiFi Network**: 2.4GHz WiFi (ESP32 doesn't support 5GHz)

---

## 📥 Installation

### Step 1: Clone Repository

```bash
git clone https://github.com/yourusername/smart-home-automation.git
cd smart-home-automation
```

### Step 2: Install Arduino IDE

1. Download from [arduino.cc](https://www.arduino.cc/en/software)
2. Install for your operating system

### Step 3: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search "ESP32" and install "ESP32 by Espressif Systems"

### Step 4: Install Libraries

**Method 1: Library Manager (Recommended)**
1. Go to **Sketch → Include Library → Manage Libraries**
2. Search and install each library listed in Software Requirements

**Method 2: Manual Installation**
1. Download libraries from GitHub
2. Extract to `Arduino/libraries/` folder

### Step 5: Configure Credentials

#### For ESP32 IoT Hub (`esp32_iot_hub/esp32_iot_hub.ino`):

```cpp
#define WIFI_SSID "Your_WiFi_Name"
#define WIFI_PASS "Your_WiFi_Password"
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your_database_secret_key"
#define BLYNK_AUTH "your_blynk_auth_token"
```

#### For ESP32-CAM (`esp32_cam_face_recognition/esp32_cam_face_recognition.ino`):

```cpp
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";
```

### Step 6: Upload Code

#### Arduino Uno:
1. Connect Arduino via USB
2. Open `arduino_local_control/arduino_local_control.ino`
3. Select **Tools → Board → Arduino Uno**
4. Select correct **Port**
5. Click **Upload**

#### ESP32:
1. Connect ESP32 via USB
2. Open `esp32_iot_hub/esp32_iot_hub.ino`
3. Select **Tools → Board → ESP32 Dev Module**
4. Select correct **Port**
5. Click **Upload**

#### ESP32-CAM:
1. Connect FTDI programmer to ESP32-CAM
2. **Connect GPIO 0 to GND** (programming mode)
3. Open `esp32_cam_face_recognition/esp32_cam_face_recognition.ino`
4. Select **Tools → Board → AI Thinker ESP32-CAM**
5. Select correct **Port**
6. Click **Upload**
7. **Disconnect GPIO 0 from GND**
8. Press **RESET** button

---

## 🔌 Wiring Diagrams

### Arduino Uno Connections

```
SENSORS:
┌─────────────────────┬──────────────┬─────────────────────────┐
│ Component           │ Arduino Pin  │ Additional Connections  │
├─────────────────────┼──────────────┼─────────────────────────┤
│ Soil Moisture       │ A0           │ VCC→5V, GND→GND        │
│ Rain Sensor         │ D7           │ VCC→5V, GND→GND        │
│ IR Motion (Room)    │ D6           │ VCC→5V, GND→GND        │
│ Ultrasonic (TRIG)   │ D9           │ VCC→5V, GND→GND        │
│ Ultrasonic (ECHO)   │ D10          │                         │
└─────────────────────┴──────────────┴─────────────────────────┘

ACTUATORS:
┌─────────────────────┬──────────────┬─────────────────────────┐
│ Component           │ Arduino Pin  │ Additional Connections  │
├─────────────────────┼──────────────┼─────────────────────────┤
│ Water Pump          │ D3           │ Via Relay IN1           │
│ Room Light          │ D4           │ Via Relay IN2           │
│ Fan (Reserved)      │ D11          │ Via Relay IN3           │
│ Gate Servo          │ D5           │ VCC→5V, GND→GND        │
│ Rain Cover Servo    │ D8           │ VCC→5V, GND→GND        │
└─────────────────────┴──────────────┴─────────────────────────┘

RELAY MODULE:
┌──────────────┬────────────────────────────────────────┐
│ Relay Pin    │ Connection                             │
├──────────────┼────────────────────────────────────────┤
│ VCC          │ Arduino 5V                             │
│ GND          │ Arduino GND                            │
│ IN1          │ Arduino D3 (Pump control)              │
│ IN2          │ Arduino D4 (Light control)             │
│ IN3          │ Arduino D11 (Fan control)              │
│ COM (Ch1)    │ Pump positive wire                     │
│ NO (Ch1)     │ Power supply positive                  │
│ COM (Ch2)    │ Light positive wire                    │
│ NO (Ch2)     │ Power supply positive                  │
└──────────────┴────────────────────────────────────────┘

SERIAL CONNECTION TO ESP32:
┌──────────────┬────────────────┐
│ Arduino      │ ESP32          │
├──────────────┼────────────────┤
│ TX (Pin 1)   │ RX2 (GPIO 16)  │
│ RX (Pin 0)   │ TX2 (GPIO 17)  │
│ GND          │ GND            │
└──────────────┴────────────────┘
```

### ESP32 IoT Hub Connections

```
┌─────────────────────┬──────────────┬─────────────────────────┐
│ Component           │ ESP32 Pin    │ Additional Connections  │
├─────────────────────┼──────────────┼─────────────────────────┤
│ DHT11 (Data)        │ GPIO 14      │ VCC→3.3V, GND→GND      │
│ MQ Smoke (Analog)   │ GPIO 34      │ VCC→5V, GND→GND        │
│ IR Motion           │ GPIO 27      │ VCC→5V, GND→GND        │
│ ESP32-CAM Trigger   │ GPIO 13      │ To ESP32-CAM GPIO 13    │
│ Arduino RX          │ GPIO 16      │ From Arduino TX         │
│ Arduino TX          │ GPIO 17      │ From Arduino RX         │
└─────────────────────┴──────────────┴─────────────────────────┘

POWER:
- ESP32: 5V via USB or VIN pin
- Sensors: Share GND with ESP32
```

### ESP32-CAM Connections

```
PERMANENT CONNECTIONS:
┌─────────────────────┬──────────────┬─────────────────────────┐
│ Component           │ ESP32-CAM    │ Notes                   │
├─────────────────────┼──────────────┼─────────────────────────┤
│ Door Lock Servo     │ GPIO 12      │ Signal pin              │
│ Servo VCC           │ 5V           │ Or external 5V supply   │
│ Servo GND           │ GND          │                         │
│ Trigger Input       │ GPIO 13      │ From main ESP32         │
│ LED Indicator       │ GPIO 33      │ Built-in or external    │
│ Power Supply        │ 5V, GND      │ 5V 2A recommended       │
└─────────────────────┴──────────────┴─────────────────────────┘

PROGRAMMING (FTDI):
┌─────────────────────┬──────────────┬─────────────────────────┐
│ FTDI Pin            │ ESP32-CAM    │ Notes                   │
├─────────────────────┼──────────────┼─────────────────────────┤
│ TX                  │ U0RXD        │ FTDI TX → CAM RX        │
│ RX                  │ U0TXD        │ FTDI RX → CAM TX        │
│ GND                 │ GND          │ Common ground           │
│ 5V                  │ 5V           │ Power supply            │
│ —                   │ GPIO 0       │ Connect to GND to upload│
└─────────────────────┴──────────────┴─────────────────────────┘

⚠️ IMPORTANT: Connect GPIO 0 to GND ONLY during code upload.
              Disconnect after upload and press RESET to run.
```

### Complete System Diagram

See `docs/wiring/complete_system_diagram.png` for visual reference.

---

## 🚀 Usage

### Initial Setup

#### 1. Power On Sequence

```
1. Connect all power supplies
2. Power on Arduino Uno
3. Power on ESP32
4. Power on ESP32-CAM
5. Wait 30 seconds for system initialization
```

#### 2. Verify System Status

**Arduino Serial Monitor (9600 baud):**
```
PUMP_OFF
LIGHT_OFF
GATE_CLOSE
COVER_OPEN
---
```

**ESP32 Serial Monitor (9600 baud):**
```
[WiFi] Connected!
[WiFi] IP Address: 192.168.1.100
[Firebase] Connected!
[Blynk] Connected!
========== SYSTEM READY ==========
```

**ESP32-CAM Serial Monitor (115200 baud):**
```
[WiFi] Connected!
[WiFi] IP Address: 192.168.1.101
========== CAMERA WEB SERVER STARTED ==========
Visit: http://192.168.1.101
========== SYSTEM READY ==========
```

### Face Enrollment (ESP32-CAM)

1. **Access Web Interface**
   - Open browser
   - Navigate to `http://[ESP32-CAM-IP]` (e.g., `http://192.168.1.101`)

2. **Enroll New Face**
   - Click **"Face Enrollment"** tab
   - Position face in camera view
   - Click **"Enroll"** button
   - Repeat 5-7 times from different angles
   - Face ID saved successfully

3. **Test Recognition**
   - Stand in front of ESP32-CAM
   - Trigger recognition (wave at IR sensor)
   - Door should unlock if face matches

### Blynk Mobile App Setup

1. **Download Blynk App**
   - iOS: App Store
   - Android: Google Play

2. **Create New Project**
   - Open Blynk app
   - Tap **"New Project"**
   - Name: "Smart Home"
   - Device: ESP32
   - Copy Auth Token (sent via email)

3. **Add Widgets**

```
┌─────────────────────────────────────────────────┐
│ Widget          │ Virtual Pin │ Configuration  │
├─────────────────┼─────────────┼────────────────┤
│ Value Display   │ V1          │ Temperature °C │
│ Terminal/Label  │ V5          │ Arduino Status │
│ Value Display   │ V6          │ Humidity %     │
│ Gauge           │ V7          │ Smoke Level    │
└─────────────────────────────────────────────────┘
```

4. **Configure Events**
   - Go to Project Settings
   - Add Event: `intruder` - "Motion detected at door"
   - Add Event: `smoke_alert` - "High smoke detected"

5. **Start Project**
   - Tap **Play** button (top right)
   - Data should start updating

### Daily Operation

#### Automatic Functions (No User Action Required)

| Function | Trigger | Action |
|----------|---------|--------|
| **Irrigation** | Soil moisture > 600 & no rain | Pump turns ON |
| **Room Light** | Motion detected | Light turns ON |
| **Parking Gate** | Vehicle < 20cm | Gate opens |
| **Rain Cover** | Rain detected | Cover closes |
| **Door Lock** | Authorized face | Door unlocks for 5s |

#### Monitoring

**Via Serial Monitor:**
- Real-time sensor readings
- System status updates
- Error messages

**Via Blynk App:**
- Temperature & humidity graphs
- Smoke level gauge
- Arduino status terminal
- Push notifications for alerts

**Via Firebase Console:**
- Historical data
- Event logs
- System status

### Manual Override

To manually control devices, you can:

1. **Via Blynk App**: Add button widgets connected to virtual pins
2. **Via Firebase**: Update values in database
3. **Via Physical Access**: Disconnect sensors temporarily

---

## 📁 Code Structure

```
smart-home-automation/
│
├── arduino_local_control/
│   └── arduino_local_control.ino      # Arduino main control code
│
├── esp32_iot_hub/
│   └── esp32_iot_hub.ino              # ESP32 IoT gateway code
│
├── esp32_cam_face_recognition/
│   └── esp32_cam_face_recognition.ino # ESP32-CAM face recognition
│
├── docs/
│   ├── wiring/
│   │   ├── arduino_connections.png
│   │   ├── esp32_connections.png
│   │   ├── esp32cam_connections.png
│   │   └── complete_system_diagram.png
│   │
│   ├── images/
│   │   ├── system-architecture.png
│   │   ├── blynk-dashboard.png
│   │   └── firebase-database.png
│   │
│   └── guides/
│       ├── TROUBLESHOOTING.md
│       ├── FIREBASE_SETUP.md
│       └── BLYNK_SETUP.md
│
├── README.md                          # This file
├── LICENSE                            # MIT License
└── .gitignore                         # Git ignore file
```

-----


## 📊 Data Flow

### Sensor Data Flow

```
Arduino Sensors → Arduino Processing → Serial → ESP32
                                                    ↓
ESP32 Sensors → ESP32 Processing → WiFi → Firebase
                                        → Blynk
                                        ↓
                                   Mobile App
```

### Control Flow

```
Blynk App → Firebase → ESP32 → Serial → Arduino → Actuators
              ↓
         Cloud Storage
```

### Face Recognition Flow

```
IR Motion (ESP32) → Trigger Signal → ESP32-CAM
                                        ↓
                                   Capture Image
                                        ↓
                                   Face Detection
                                        ↓
                                   Face Recognition
                                        ↓
                          Match? → Yes → Unlock Door
                                  ↓
                                  No → Keep Locked
```

---

## 🔐 Security Considerations

### Network Security
- Use WPA2/WPA3 encryption on WiFi
- Change default Firebase database rules
- Use strong passwords for all services
- Consider VPN for remote access

### Physical Security
- Enclose ESP32-CAM in weatherproof housing
- Hide wiring and modules from tampering
- Add tamper detection sensors
- Use metal enclosures for critical components

### Face Recognition Limitations
- Can be fooled by high-quality photos
- Lighting affects accuracy
- Consider adding liveness detection
- Implement access logging
- Add PIN backup method

### Data Privacy
- Face data stored locally (not in cloud)
- Review Firebase security rules
- Limit Blynk app access
- Regularly update Auth Tokens

---


## 🙏 Acknowledgments

- Arduino Community for extensive libraries
- Espressif for ESP32 platform
- Mobizt for FirebaseESP32 library
- Blynk team for IoT platform
- Contributors and testers


## 

### Blynk Mobile Dashboard
![Blynk Dashboard](![WhatsApp Image 2026-02-07 at 21 06 53](https://github.com/user-attachments/assets/0ca07591-1b04-4fc5-a33d-ced3fbf99e60)
)

### Firebase Realtime Database
![Firebase Database](<img width="1354" height="736" alt="Screenshot 2026-02-07 205316" src="https://github.com/user-attachments/assets/941c0e06-5702-4f4c-b850-7057894ea723" />)

### Picture of House model Describing all features
![House Model ](<img width="393" height="311" alt="Screenshot 2026-02-07 192730" src="https://github.com/user-attachments/assets/2edec5a8-6c68-4eed-b081-078927296919" />)

### Working demo video of all features
![Smart Home Automation](https://github.com/user-attachments/assets/711887a0-803e-41db-bcc4-a451ef9558c8)




---

## ⚡ Performance Metrics

- **Response Time**: < 1 second for sensor triggers
- **WiFi Range**: Up to 50 meters (indoor)
- **Face Recognition Accuracy**: ~95% (proper lighting)
- **System Uptime**: 99%+ (with stable power)
- **Data Update Frequency**: Every 2 seconds
- **Power Consumption**: ~5W total (all modules)

----
**Author**
---
KOLLI VENNELA CHOWDARY

------
**Liscence**
---
This project is intended for educational purposes.

