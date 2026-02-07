# Quick Start Guide

Get your Smart Home Automation System up and running in 30 minutes!

---

## ⚡ What You'll Need

- ✅ All components assembled (see [BILL_OF_MATERIALS.md])
- ✅ Arduino IDE installed
- ✅ USB cables for programming
- ✅ WiFi credentials ready
- ✅ 30-60 minutes of time

---

## 🚀 Fast Track Setup (3 Steps)

### Step 1: Install Arduino IDE & Libraries (10 min)

1. **Download Arduino IDE**
   - Go to: https://www.arduino.cc/en/software
   - Install for your OS (Windows/Mac/Linux)

2. **Add ESP32 Board Support**
   - Open Arduino IDE
   - File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager
   - Search "ESP32" → Install "ESP32 by Espressif Systems"

3. **Install Libraries**
   - Sketch → Include Library → Manage Libraries
   - Install these libraries:
     ```
     - FirebaseESP32 (by Mobizt)
     - Blynk (by Volodymyr Shymanskyy)
     - DHT sensor library (by Adafruit)
     - Adafruit Unified Sensor
     - ESP32Servo
     ```

---

### Step 2: Wire Up & Upload Code (15 min)

#### A. Arduino Uno Setup

**Quick Wiring:**
```
Soil Sensor    → A0, VCC(5V), GND
Rain Sensor    → D7, VCC(5V), GND
IR Motion      → D6, VCC(5V), GND
Ultrasonic     → D9(TRIG), D10(ECHO), VCC(5V), GND
Gate Servo     → D5 (signal), VCC(5V), GND
Rain Servo     → D8 (signal), VCC(5V), GND
Relay Module   → D3, D4, D11, VCC(5V), GND
```

**Upload Code:**
1. Connect Arduino via USB
2. Open `arduino_local_control.ino`
3. Tools → Board → Arduino Uno
4. Tools → Port → (select COM port)
5. Click Upload ⬆️
6. Wait for "Done uploading"

**Test:**
- Open Serial Monitor (9600 baud)
- Should see: `PUMP_OFF`, `LIGHT_OFF`, etc.

---

#### B. ESP32 Setup

**Quick Wiring:**
```
DHT11          → GPIO14, VCC(3.3V), GND
MQ Smoke       → GPIO34, VCC(5V), GND
IR Motion      → GPIO27, VCC(5V), GND
Arduino TX     → GPIO16 (RX2)
Arduino RX     → GPIO17 (TX2)
ESP32-CAM      → GPIO13
All GND        → Common GND
```

**Update Credentials:**
1. Open `esp32_iot_hub.ino`
2. Replace:
   ```cpp
   #define WIFI_SSID "YOUR_WIFI_SSID"
   #define WIFI_PASS "YOUR_WIFI_PASSWORD"
   #define FIREBASE_HOST "your-project.firebaseio.com"
   #define FIREBASE_AUTH "your_firebase_secret"
   #define BLYNK_AUTH "your_blynk_token"
   ```

**Upload Code:**
1. Connect ESP32 via USB
2. Tools → Board → ESP32 Dev Module
3. Tools → Port → (select COM port)
4. Click Upload ⬆️
5. Wait for "Done uploading"

**Test:**
- Open Serial Monitor (9600 baud)
- Should see: `[WiFi] Connected!`, `[Firebase] Connected!`

---

#### C. ESP32-CAM Setup

**Quick Wiring (Programming):**
```
FTDI → ESP32-CAM
TX   → U0RXD
RX   → U0TXD
GND  → GND
5V   → 5V
GPIO 0 → GND (only during upload!)
```

**Update WiFi:**
1. Open `esp32_cam_face_recognition.ino`
2. Replace:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

**Upload Code:**
1. Connect FTDI to ESP32-CAM
2. **Connect GPIO 0 to GND**
3. Connect power (5V)
4. Tools → Board → AI Thinker ESP32-CAM
5. Tools → Port → (select COM port)
6. Click Upload ⬆️
7. Wait for "Done uploading"
8. **Disconnect GPIO 0 from GND**
9. Press RESET button

**Test:**
- Open Serial Monitor (115200 baud)
- Should see: IP address (e.g., `192.168.1.101`)
- Open browser → http://[IP_ADDRESS]

---

### Step 3: Configure Cloud Services (5 min)

#### Firebase Setup (2 min)

1. Go to: https://console.firebase.google.com
2. Create project → Name it (e.g., "smart-home")
3. Build → Realtime Database → Create Database
4. Start in **test mode** → Enable
5. Copy Database URL (without https://)
6. Project Settings → Service Accounts → Database Secrets → Show
7. Copy secret key
8. Update in ESP32 code (already done in Step 2B)

---

#### Blynk Setup (3 min)

1. Download Blynk app (iOS/Android)
2. Create account → Sign in
3. Web console: https://blynk.cloud
4. New Template → Name: "Smart Home"
5. Hardware: ESP32 → Save
6. Devices → New Device → Create
7. Copy Auth Token
8. Update in ESP32 code (already done in Step 2B)

**Add Widgets:**
- V1: Value Display (Temperature)
- V5: Label (Arduino Status)
- V6: Value Display (Humidity)
- V7: Gauge (Smoke Level)

---

## ✅ Verification Checklist

After setup, verify each component:

```
□ Arduino powers on (LED blinks)
□ ESP32 connects to WiFi
□ ESP32-CAM shows IP address
□ Firebase shows data in console
□ Blynk app shows device "Online"
□ Sensors provide readings
□ Actuators respond to triggers
```

---

## 🧪 Quick Tests

### Test 1: Arduino Sensors
```
1. Cover soil sensor → Should see PUMP_ON in Serial Monitor
2. Wave hand over IR sensor → Should see LIGHT_ON
3. Place object near ultrasonic → Should see GATE_OPEN
4. Wet rain sensor → Should see COVER_CLOSE
```

### Test 2: ESP32 IoT
```
1. Check Serial Monitor → Should see sensor readings every 2 seconds
2. Open Firebase Console → Data should update in real-time
3. Open Blynk app → Widgets should show live data
4. Wave at IR sensor → Should get notification
```

### Test 3: Face Recognition
```
1. Open browser → http://[ESP32-CAM-IP]
2. See camera feed
3. Click "Face Enrollment" → Enroll your face (5 times)
4. Trigger ESP32 (wave at main IR sensor)
5. ESP32-CAM should detect and recognize face
6. Door servo should unlock
```

---

## 🐛 Common First-Time Issues

### Arduino Issues

**Problem:** Port not showing in Arduino IDE
```
Solution:
→ Install Arduino USB drivers
→ Try different USB cable
→ Check Device Manager (Windows)
```

**Problem:** Upload fails
```
Solution:
→ Close Serial Monitor before uploading
→ Select correct board (Arduino Uno)
→ Try different USB port
```

---

### ESP32 Issues

**Problem:** WiFi won't connect
```
Solution:
→ Double-check SSID and password (case-sensitive!)
→ Ensure 2.4GHz WiFi (not 5GHz)
→ Move closer to router
```

**Problem:** Firebase error "Permission Denied"
```
Solution:
→ Set database rules to test mode:
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

**Problem:** Blynk shows "Offline"
```
Solution:
→ Verify Auth Token is correct
→ Check WiFi connection first
→ Ensure project is not in "STOP" mode
```

---

### ESP32-CAM Issues

**Problem:** Can't upload code
```
Solution:
→ GPIO 0 must be connected to GND during upload
→ Press RESET button before uploading
→ Select "AI Thinker ESP32-CAM" board
→ Try upload speed 115200
```

**Problem:** "Brown-out detector" error
```
Solution:
→ CRITICAL: Use 5V 2A power supply
→ DO NOT use USB power or Arduino 5V
→ Add 470µF capacitor near power pins
```

**Problem:** Camera init failed
```
Solution:
→ Check camera ribbon cable connection
→ Ensure OV2640 camera (not OV7670)
→ Reseat camera module
```

---

## 📱 Mobile App Setup (Blynk)

### Quick Widget Setup

1. **Temperature Display**
   - Widget: Value Display
   - Pin: V1
   - Range: -20 to 50°C

2. **Humidity Display**
   - Widget: Gauge
   - Pin: V6
   - Range: 0 to 100%

3. **Smoke Level**
   - Widget: Gauge
   - Pin: V7
   - Range: 0 to 4095
   - Color zones: Green (0-200), Yellow (200-400), Red (400+)

4. **Status Terminal**
   - Widget: Label
   - Pin: V5

---

## 🔐 Security Note

**For testing only:**
- Firebase rules set to public read/write
- No authentication required

**For production:**
- Enable Firebase Authentication
- Restrict database rules
- Use strong WiFi password
- Change default credentials

---

## 📊 Expected Results

After successful setup:

### Serial Monitor Output

**Arduino (9600 baud):**
```
PUMP_OFF|Soil_Moist
LIGHT_OFF
GATE_CLOSE
COVER_OPEN
---
```

**ESP32 (9600 baud):**
```
[WiFi] Connected!
[WiFi] IP Address: 192.168.1.100
[Firebase] Connected!
[Blynk] Connected!
[DHT11] Temp: 28.3°C | Humidity: 65.5%
[Smoke] Level: 145
```

**ESP32-CAM (115200 baud):**
```
[WiFi] Connected!
[WiFi] IP Address: 192.168.1.101
[Camera] Initialized successfully
Visit: http://192.168.1.101
========== SYSTEM READY ==========
```

---

### Firebase Console
```
home/
  ├── status: "PUMP_OFF"
  ├── temperature: 28.3
  ├── humidity: 65.5
  └── smoke: 145
```

---

### Blynk App
- Device status: **Online** (green)
- Temperature widget: **28.3°C**
- Humidity widget: **65.5%**
- Smoke gauge: **145** (green zone)

---

## 🎯 Next Steps

Once basic system is working:

1. ✅ **Test all sensors individually**
2. ✅ **Calibrate sensor thresholds** (soil moisture, smoke)
3. ✅ **Enroll faces in ESP32-CAM**
4. ✅ **Set up Blynk notifications/events**
5. ✅ **Create automations in Blynk**
6. ✅ **Add enclosures for weather protection**
7. ✅ **Implement proper security** (Firebase rules, auth)

---

## 📚 Full Documentation

For detailed setup and troubleshooting:

- **README.md** - Complete project overview
- **FIREBASE_SETUP.md** - Firebase configuration
- **BLYNK_SETUP.md** - Blynk mobile app setup
- **TROUBLESHOOTING.md** - Common issues and fixes
- **BILL_OF_MATERIALS.md** - Component list and suppliers

---

## 📞 Get Help

Having issues? Check:

1. **Serial Monitor** - Look for error messages
2. **Wiring Diagram** - Verify all connections
3. **Troubleshooting Guide** - Common solutions
4. **GitHub Issues** - Search existing problems
5. **Community Forums** - Ask for help

---

## ⏱️ Setup Time Breakdown

- **Arduino**: 5-10 minutes
- **ESP32**: 10-15 minutes
- **ESP32-CAM**: 10-15 minutes
- **Cloud Services**: 5-10 minutes
- **Testing**: 10-15 minutes

**Total: 30-60 minutes** (depending on experience)

---

**Congratulations! Your Smart Home Automation System should now be operational! 🎉**

---
