# Troubleshooting Guide

Complete troubleshooting reference for Smart Home Automation System issues.

---

## 📋 Quick Diagnosis

### System Not Working At All
1. Check all power supplies connected
2. Verify USB/power cables are good quality
3. Look for LED indicators on boards
4. Check Serial Monitor for error messages

### Partial System Working
1. Identify which module is failing (Arduino/ESP32/ESP32-CAM)
2. Test each module independently
3. Check inter-module connections
4. Verify common ground connections

---

## 🔧 Arduino Uno Issues

### Issue 1: Arduino Not Detected by Computer

**Symptoms:**
- No COM port appears
- "Device not recognized" error
- Upload fails immediately

**Solutions:**
```
✓ Try different USB cable (data-capable, not charge-only)
✓ Try different USB port
✓ Install/update Arduino drivers
✓ Check Device Manager (Windows) for unknown devices
✓ Try another computer to isolate issue
✓ Check if Arduino LED blinks during upload
```

**Driver Installation:**
- Windows: Arduino IDE auto-installs drivers
- Mac: No drivers needed
- Linux: May need to add user to `dialout` group
  ```bash
  sudo usermod -a -G dialout $USER
  ```

---

### Issue 2: Sensors Not Reading Correctly

#### Soil Moisture Always Dry/Wet

**Solutions:**
```
✓ Check sensor is inserted in soil (not air)
✓ Verify VCC and GND connections
✓ Test sensor with multimeter
✓ Calibrate threshold value in code
✓ Replace sensor if corroded
```

**Calibration:**
```cpp
// Test in Serial Monitor
int soil = analogRead(SOIL);
Serial.println(soil);

// Dry soil: typically 600-1023
// Wet soil: typically 200-400
// Adjust threshold based on your readings
```

#### Rain Sensor Always Triggered

**Solutions:**
```
✓ Clean sensor surface (no water drops)
✓ Adjust sensitivity potentiometer
✓ Check for condensation
✓ Verify Digital Output (DO) connected, not Analog (AO)
✓ Test with intentional water drops
```

#### Ultrasonic Sensor Reading 0 or Random Values

**Solutions:**
```
✓ Check TRIG → D9, ECHO → D10 connections
✓ Ensure 5V power (not 3.3V)
✓ Keep sensor away from soft/angled surfaces
✓ Maximum range is ~4 meters
✓ Add timeout to pulseIn (already in code)
✓ Test with simple UltrasonicDistance example
```

**Test Code:**
```cpp
void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH, 30000);
  int distance = duration * 0.034 / 2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(500);
}
```

#### IR Motion Sensor Always Triggering

**Solutions:**
```
✓ Adjust sensitivity potentiometer (usually on sensor)
✓ Adjust delay potentiometer
✓ Remove heat sources nearby (lamps, heaters)
✓ Ensure sensor is mounted stable (not vibrating)
✓ Check for direct sunlight
✓ Verify 5V power
```

---

### Issue 3: Actuators Not Working

#### Water Pump Not Running

**Check List:**
```
✓ Relay clicking sound when triggered?
✓ LED on relay module lights up?
✓ Pump power supply connected?
✓ Test pump with direct power (bypass relay)
✓ Check relay NO (Normally Open) terminal used
✓ Verify relay can handle pump current
✓ Test relay with separate Arduino sketch
```

**Test Relay Code:**
```cpp
void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  digitalWrite(3, HIGH);
  delay(2000);
  digitalWrite(3, LOW);
  delay(2000);
}
```

#### Room Light Not Turning On

**Solutions:**
```
✓ Same checks as pump above
✓ Verify light bulb is good
✓ Check AC wiring polarity if using AC
✓ Ensure relay rated for AC if using AC loads
✓ Test with LED first (safer)
```

#### Servo Motors Jittering or Not Moving

**Symptoms:**
- Servo shakes but doesn't move
- Erratic movement
- Servo doesn't hold position

**Solutions:**
```
✓ Use separate 5V power supply for servos (not Arduino 5V)
✓ Add 100µF-470µF capacitor across servo power
✓ Keep servo wires short (<15cm)
✓ Use quality servos (SG90 or better)
✓ Don't exceed servo torque limits
✓ Check servo isn't mechanically blocked
```

**Power Circuit:**
```
External 5V 2A ──┬─→ Servo VCC (Red)
                 │
Arduino GND ─────┴─→ Servo GND (Brown)
                 └─→ Power Supply GND

Arduino D5/D8 ────→ Servo Signal (Orange/Yellow)
```

**Test Servo Code:**
```cpp
#include <Servo.h>
Servo test;

void setup() {
  test.attach(5);
}

void loop() {
  test.write(0);
  delay(1000);
  test.write(90);
  delay(1000);
  test.write(180);
  delay(1000);
}
```

---

### Issue 4: Serial Communication Problems

#### No Data in Serial Monitor

**Solutions:**
```
✓ Verify baud rate is 9600 (match in code and monitor)
✓ Select correct COM port
✓ Check "No line ending" / "Newline" setting
✓ Ensure Serial.begin(9600) is in setup()
✓ Add Serial.println() statements for debugging
```

#### Garbled Data in Serial Monitor

**Cause:** Baud rate mismatch

**Solution:**
```cpp
// In Arduino code
Serial.begin(9600);

// In Serial Monitor
Set to 9600 baud (bottom right dropdown)
```

#### Arduino-ESP32 Serial Communication Failing

**Check:**
```
✓ Arduino TX(D1) → ESP32 RX2(GPIO16)
✓ Arduino RX(D0) → ESP32 TX2(GPIO17)  [Crossover!]
✓ Common GND between Arduino and ESP32
✓ Both using same baud rate (9600)
✓ No Serial Monitor open on Arduino (conflicts with ESP32)
```

---

## 🌐 ESP32 IoT Hub Issues

### Issue 1: WiFi Not Connecting

**Symptoms:**
- `[WiFi] Connection FAILED!` in Serial Monitor
- Stuck at connecting dots

**Solutions:**
```
✓ Verify SSID and password are correct (case-sensitive!)
✓ Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
✓ Move ESP32 closer to router
✓ Check router settings (MAC filtering disabled)
✓ Try different WiFi network
✓ Restart router
✓ Check WiFi signal strength
```

**Test Code:**
```cpp
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(" Status: ");
    Serial.println(WiFi.status());
  }
  
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
}
```

**WiFi Status Codes:**
```
0 = WL_IDLE_STATUS
1 = WL_NO_SSID_AVAIL (SSID not found)
3 = WL_CONNECTED
4 = WL_CONNECT_FAILED (Wrong password)
6 = WL_DISCONNECTED
```

---

### Issue 2: Firebase Connection Failed

**Symptoms:**
- `[Firebase] Upload failed: ...` errors
- Data not appearing in Firebase Console

**Solutions:**
```
✓ Verify FIREBASE_HOST format (no https://, no trailing /)
   Correct: "project-id.firebaseio.com"
   Wrong:   "https://project-id.firebaseio.com/"
✓ Check FIREBASE_AUTH (database secret) is correct
✓ Verify Firebase database rules allow read/write
✓ Ensure internet connection is working
✓ Check Firebase project is active
✓ Update FirebaseESP32 library to latest version
```

**Test Firebase Connection:**
```cpp
if (Firebase.setString(fbdo, "/test", "Hello")) {
  Serial.println("Firebase OK");
} else {
  Serial.println("Error: " + fbdo.errorReason());
}
```

**Common Error Reasons:**
```
"auth token is invalid" → Check FIREBASE_AUTH
"permission denied" → Check database rules
"connection timeout" → Check internet connection
```

**Database Rules for Testing:**
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

---

### Issue 3: Blynk Not Connecting

**Symptoms:**
- `[Blynk] Connection FAILED!` in Serial Monitor
- Device shows "Offline" in Blynk app

**Solutions:**
```
✓ Verify BLYNK_AUTH token is correct (32 characters)
✓ Check WiFi is connected first
✓ Ensure Blynk server is reachable (blynk.cloud)
✓ Verify project is not in "STOP" mode in app
✓ Try regenerating Auth Token
✓ Check Blynk account is active
```

**Test Blynk Connection:**
```cpp
Blynk.config(BLYNK_AUTH);
if (Blynk.connect()) {
  Serial.println("Blynk connected!");
} else {
  Serial.println("Blynk connection failed");
}
```

---

### Issue 4: DHT11 Sensor Returning NaN

**Symptoms:**
- Temperature and humidity show "nan" or invalid values
- Inconsistent readings

**Solutions:**
```
✓ Check wiring: VCC→3.3V, GND→GND, DATA→GPIO14
✓ Add 10kΩ pull-up resistor between DATA and VCC
✓ Wait 2 seconds between readings (DHT11 is slow)
✓ Try different DHT11 sensor (may be faulty)
✓ Ensure DHT library is installed correctly
✓ Try DHT22 instead (more reliable)
```

**Pull-up Resistor:**
```
VCC (3.3V) ──┬─→ DHT11 VCC
             │
            [10kΩ]
             │
             ├─→ DHT11 DATA → GPIO14
             
GND ─────────┴─→ DHT11 GND
```

**Test DHT Code:**
```cpp
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read failed!");
  } else {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print("°C, Humidity: ");
    Serial.print(h);
    Serial.println("%");
  }
}
```

---

### Issue 5: MQ Smoke Sensor Issues

#### Always Reading High Values

**Solutions:**
```
✓ Sensor needs "burn-in" period (24-48 hours powered on)
✓ Allow 30 seconds warm-up before each reading
✓ Check if sensor is near smoke/gas source
✓ Calibrate threshold based on environment
✓ Ensure good ventilation during testing
```

#### Reading 0 or Very Low

**Solutions:**
```
✓ Check VCC→5V (not 3.3V, MQ sensors need 5V)
✓ Verify AO (analog output) connected to GPIO34
✓ Sensor may be faulty or expired
✓ Check for loose connections
```

**Smoke Sensor Calibration:**
```cpp
void loop() {
  int smoke = analogRead(34);
  Serial.print("Smoke: ");
  Serial.println(smoke);
  
  // Typical values:
  // Clean air: 0-100
  // Light smoke: 100-300
  // Heavy smoke: 300+
  
  delay(1000);
}
```

---

## 📹 ESP32-CAM Issues

### Issue 1: Can't Upload Code

**Symptoms:**
- "Failed to connect" error
- Upload hangs at "Connecting..."

**Solutions:**
```
✓ Connect GPIO 0 to GND BEFORE powering on
✓ Press and hold RESET button while uploading
✓ Use quality USB cable (data-capable)
✓ Install correct drivers (CP2102/CH340)
✓ Select board: AI Thinker ESP32-CAM
✓ Try lower upload speed: 115200 (not 921600)
✓ Check FTDI programmer is set to 5V (not 3.3V)
```

**Upload Procedure:**
```
1. Connect FTDI to ESP32-CAM
2. Connect GPIO 0 to GND
3. Connect power (5V)
4. Press RESET button
5. Click Upload in Arduino IDE
6. Wait for "Connecting..."
7. When upload starts, release RESET
8. Wait for "Hard resetting..."
9. Disconnect GPIO 0 from GND
10. Press RESET to run program
```

---

### Issue 2: Brown-out Detector Triggered

**Symptoms:**
- Serial Monitor shows "Brownout detector was triggered"
- ESP32-CAM keeps resetting
- Can't initialize camera

**Cause:** Insufficient or unstable power supply

**Solutions:**
```
✓ Use 5V 2A power supply (minimum 1A)
✓ DO NOT power from Arduino 5V pin
✓ DO NOT power from USB-TTL 3.3V
✓ Use quality power adapter
✓ Add 470µF capacitor near ESP32-CAM power pins
✓ Use thick, short power wires (lower resistance)
✓ Try different power supply
```

**Proper Power Setup:**
```
5V 2A Adapter ──┬─→ ESP32-CAM 5V
                └─→ ESP32-CAM GND

Add capacitor:
     ┌─[470µF]─┐
     │         │
    5V        GND
```

---

### Issue 3: Camera Init Failed

**Symptoms:**
- `[ERROR] Camera init failed: 0x105` or similar
- Camera not detected

**Solutions:**
```
✓ Check camera ribbon cable connection (both ends)
✓ Ensure camera is OV2640 model (not OV7670)
✓ Reseat camera module gently
✓ Check for damage to ribbon cable
✓ Verify correct camera pins definition
✓ Try different ESP32-CAM board
✓ Check PSRAM is enabled (board settings)
```

**Camera Connection:**
```
Camera ribbon cable has two ends:
- One end: Camera module (blue side faces camera)
- Other end: ESP32-CAM board (blue side faces away from board)

Gently insert until it clicks, then close latch
```

---

### Issue 4: Face Not Detected

**Symptoms:**
- `[Result] No face detected` always
- Face recognition never triggers

**Solutions:**
```
✓ Improve lighting (face should be well-lit from front)
✓ Correct distance: 30-100cm from camera
✓ Face camera directly (not angled)
✓ Remove glasses, hat, or mask
✓ Ensure camera focus is correct (may need manual adjustment)
✓ Check camera isn't covered or dirty
✓ Try different background (plain is better)
```

**Lighting Tips:**
```
Good lighting:
- Natural daylight
- Bright indoor lighting from front
- Avoid backlighting (light behind person)

Bad lighting:
- Very dim lighting
- Strong backlight
- Harsh shadows on face
```

---

### Issue 5: Face Detected But Not Recognized

**Symptoms:**
- `[Detected] 1 face(s) found`
- `❌ UNKNOWN FACE - ACCESS DENIED`
- Even though face is enrolled

**Solutions:**
```
✓ Re-enroll face from multiple angles (5-7 times)
✓ Enroll in similar lighting as actual use
✓ Ensure enrollment completed successfully
✓ Lower recognition threshold (in library settings)
✓ Clear old enrollments and start fresh
✓ Check if multiple people enrolled (max 7)
✓ Ensure face is visible (not partially covered)
```

**Enrollment Best Practices:**
```
1. Good, even lighting
2. Face camera directly
3. 50-70cm distance
4. Enroll 5-7 times:
   - Front view (2 times)
   - Slight left turn
   - Slight right turn
   - Slight up angle
   - Slight down angle
5. Same expression each time
6. Remove glasses for one enrollment
```

---

### Issue 6: Servo Not Moving (Door Lock)

**Solutions:**
```
✓ Check servo wiring to GPIO 12
✓ Use external 5V supply for servo (not ESP32-CAM 5V)
✓ Ensure common GND between servo and ESP32-CAM
✓ Test servo with simple sweep code
✓ Check servo isn't mechanically blocked
✓ Verify servo is getting enough current
```

**Test Servo:**
```cpp
#include <ESP32Servo.h>
Servo test;

void setup() {
  test.attach(12);
}

void loop() {
  test.write(0);
  delay(1000);
  test.write(90);
  delay(1000);
}
```

---

### Issue 7: Web Interface Not Accessible

**Symptoms:**
- Can't open `http://[ESP32-CAM-IP]`
- "Site can't be reached" error

**Solutions:**
```
✓ Verify ESP32-CAM connected to WiFi
✓ Check IP address in Serial Monitor
✓ Ensure computer on same network
✓ Try different browser
✓ Disable firewall temporarily
✓ Check WiFi router doesn't block device-to-device
```

**Find IP Address:**
```cpp
// In Serial Monitor (115200 baud):
[WiFi] IP Address: 192.168.1.101

// Then open browser:
http://192.168.1.101
```

---

## 🔗 Integration Issues

### Issue: Arduino and ESP32 Not Communicating

**Check:**
```
✓ Arduino TX → ESP32 RX2 (GPIO 16)
✓ Arduino RX → ESP32 TX2 (GPIO 17)
✓ Common GND connection
✓ Both set to 9600 baud
✓ No Serial Monitor open on Arduino while ESP32 connected
```

**Test Serial Communication:**

**Arduino Code:**
```cpp
void loop() {
  Serial.println("TEST");
  delay(1000);
}
```

**ESP32 Code:**
```cpp
void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(msg);
  }
}
```

---

### Issue: ESP32-CAM Not Triggered

**Symptoms:**
- IR sensor detects motion
- ESP32-CAM never activates

**Check:**
```
✓ ESP32 GPIO 13 → ESP32-CAM GPIO 13 connected
✓ Common GND between ESP32 and ESP32-CAM
✓ IR sensor on ESP32 is working
✓ Trigger signal code is executing
```

**Debug ESP32 Trigger:**
```cpp
if (digitalRead(IR_MAIN) == LOW) {
  Serial.println("IR triggered - sending pulse");
  digitalWrite(CAM_TRIGGER, HIGH);
  delay(100);
  digitalWrite(CAM_TRIGGER, LOW);
}
```

**Debug ESP32-CAM Receive:**
```cpp
void loop() {
  int trigger = digitalRead(TRIGGER_PIN);
  Serial.print("Trigger pin state: ");
  Serial.println(trigger);
  delay(500);
}
```

---

## 🔋 Power Issues

### Brownouts and Resets

**Symptoms:**
- Random resets
- "Brownout detector" messages
- Devices not working reliably

**Solutions:**
```
✓ Use adequate power supplies:
  - Arduino: 9-12V 1A
  - ESP32: 5V 2A
  - ESP32-CAM: 5V 2A (critical!)
✓ Separate power for servos and motors
✓ Add bulk capacitors (470µF-1000µF) near power inputs
✓ Use quality USB cables (thick, short)
✓ Don't daisy-chain power
```

**Power Budget:**
```
Arduino Uno: ~500mA
ESP32: ~500mA (WiFi active)
ESP32-CAM: ~500mA (camera active)
Servo (SG90): ~200mA each
Relay module: ~100mA
Sensors: ~50mA total

Minimum total: ~2A
Recommended: 3-4A total capacity
```

---

## 📊 Debugging Techniques

### Serial Monitor Debugging

**Add debug output:**
```cpp
Serial.print("Variable name: ");
Serial.println(variable);
Serial.println("Checkpoint reached");
```

**Track execution flow:**
```cpp
void loop() {
  Serial.println("Loop start");
  
  // Your code here
  Serial.println("After sensor read");
  
  // More code
  Serial.println("Loop end");
}
```

---

### LED Indicators

**Use built-in LED for status:**
```cpp
#define DEBUG_LED 2

void setup() {
  pinMode(DEBUG_LED, OUTPUT);
}

// Blink to indicate state
digitalWrite(DEBUG_LED, HIGH);
delay(100);
digitalWrite(DEBUG_LED, LOW);
```

---

### Multimeter Testing

**Check voltages:**
```
5V rail should be: 4.5V - 5.5V
3.3V rail should be: 3.0V - 3.6V
Sensor outputs: varies by sensor
```

**Check continuity:**
```
Test GND connections between modules
Test signal wires for breaks
Check for shorts (should be open circuit)
```

---

## 📞 Getting Help

### Before Asking for Help

Gather this information:
```
1. Which module is failing? (Arduino/ESP32/ESP32-CAM)
2. What were you doing when it failed?
3. Error messages from Serial Monitor
4. Which code version are you using?
5. Have you made any modifications?
6. Power supply specifications
7. Component models (sensors, boards)
8. Photos of wiring (if applicable)
```

### Where to Get Help

- **GitHub Issues**: Post detailed issue with logs
- **Arduino Forum**: forum.arduino.cc
- **ESP32 Forum**: esp32.com
- **Reddit**: r/arduino, r/esp32
- **Project Documentation**: Check README and guides

---

## ✅ Systematic Troubleshooting Checklist

For any issue, work through this list:

```
□ Check power supply to all modules
□ Verify all connections (especially GND)
□ Check Serial Monitor for error messages
□ Test components individually
□ Verify code uploaded successfully
□ Check credentials (WiFi, Firebase, Blynk)
□ Review wiring against diagrams
□ Test with simple example code
□ Check for physical damage
□ Try swapping suspect components
```

---

**Remember:** 90% of issues are wiring or power related. Always double-check connections!
