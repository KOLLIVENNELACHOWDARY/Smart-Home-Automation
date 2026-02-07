# System Workflow & Architecture

Detailed explanation of how the Smart Home Automation System works, including data flow, control logic, and system interactions.

---

## 📊 System Overview

The system operates as a **three-tier architecture**:

1. **Tier 1: Local Control (Arduino)** - Real-time sensor monitoring and actuator control
2. **Tier 2: IoT Gateway (ESP32)** - Cloud connectivity and additional monitoring
3. **Tier 3: Security Layer (ESP32-CAM)** - AI-powered access control

---

## 🔄 Complete System Workflow

### System Startup Sequence

```
1. Power On All Modules
   ├── Arduino Uno initializes
   │   ├── Configures all sensor pins
   │   ├── Configures all actuator pins
   │   ├── Initializes servo motors (gate & rain cover)
   │   └── Opens Serial communication (9600 baud)
   │
   ├── ESP32 initializes
   │   ├── Connects to WiFi network
   │   ├── Establishes Firebase connection
   │   ├── Connects to Blynk server
   │   ├── Initializes DHT11 sensor
   │   └── Opens Serial2 for Arduino communication
   │
   └── ESP32-CAM initializes
       ├── Initializes OV2640 camera
       ├── Connects to WiFi network
       ├── Starts web server for face enrollment
       ├── Loads face recognition models
       └── Waits for trigger signal

2. System Ready
   └── All modules report "SYSTEM READY"
```

---

## 🌱 Feature 1: Automatic Irrigation System

### Workflow Diagram

```
┌─────────────────────────────────────────────────────────┐
│                  IRRIGATION CYCLE                        │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Read Soil Moisture    │
              │ (Analog A0)          │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Is Soil Dry?         │
              │ (value > 600)        │
              └───────┬───────┬───────┘
                      │       │
                   NO │       │ YES
                      │       │
                      ▼       ▼
             ┌────────┐   ┌────────────┐
             │ Skip   │   │ Read Rain  │
             │        │   │ Sensor     │
             └────────┘   └──────┬─────┘
                                 │
                                 ▼
                      ┌──────────────────┐
                      │ Is it Raining?   │
                      │ (sensor == LOW)  │
                      └────┬─────┬───────┘
                           │     │
                        YES│     │NO
                           │     │
                           ▼     ▼
                    ┌──────┐  ┌─────────────┐
                    │ Skip │  │ Turn Pump ON│
                    │      │  │ (D3 = HIGH) │
                    └──────┘  └──────┬──────┘
                                     │
                                     ▼
                         ┌────────────────────┐
                         │ Send Status to ESP32│
                         │ "PUMP_ON|Soil:650"│
                         └────────┬───────────┘
                                  │
                                  ▼
                         ┌────────────────────┐
                         │ ESP32 → Firebase   │
                         │ ESP32 → Blynk      │
                         └────────────────────┘
```

### Logic Code Reference

```cpp
// Arduino: arduino_local_control.ino
int soil = analogRead(SOIL);        // 0-1023
int rain = digitalRead(RAIN);        // HIGH = no rain, LOW = rain

if (soil > 600 && rain == HIGH) {   // Dry soil AND no rain
    digitalWrite(PUMP, HIGH);        // Turn pump ON
    Serial.println("PUMP_ON|Soil:" + String(soil));
} else {
    digitalWrite(PUMP, LOW);         // Turn pump OFF
}
```

### Threshold Calibration

| Soil Condition | Analog Reading | Action |
|----------------|----------------|--------|
| Very Wet | 200-400 | Pump OFF |
| Moist | 400-600 | Pump OFF |
| **Dry** | **600-800** | **Pump ON** (if no rain) |
| Very Dry | 800-1023 | Pump ON (if no rain) |

---

## 💡 Feature 2: Motion-Activated Lighting

### Workflow Diagram

```
┌─────────────────────────────────────────────────────────┐
│                 LIGHTING CONTROL CYCLE                   │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Read IR Motion Sensor │
              │ (Digital D6)         │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Motion Detected?     │
              │ (sensor == LOW)      │
              └───────┬───────┬───────┘
                      │       │
                   NO │       │ YES
                      │       │
                      ▼       ▼
          ┌──────────────┐  ┌─────────────┐
          │ Turn Light   │  │ Turn Light  │
          │ OFF          │  │ ON          │
          │ (D4 = LOW)   │  │ (D4 = HIGH) │
          └──────┬───────┘  └──────┬──────┘
                 │                 │
                 └────────┬────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Send Status to ESP32  │
              │ "LIGHT_ON|Motion"    │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ ESP32 → Firebase      │
              │ ESP32 → Blynk         │
              └───────────────────────┘
```

### PIR Sensor Behavior

**Trigger Mode:** Repeatable
- Motion detected → Output HIGH for configured delay
- Continuous motion → Output stays HIGH
- No motion → Output goes LOW after delay

**Typical Settings:**
- Sensitivity: 3-7 meters
- Delay: 2-5 seconds
- Detection angle: 120°

---

## 🚗 Feature 3: Automatic Parking Gate

### Workflow Diagram

```
┌─────────────────────────────────────────────────────────┐
│               PARKING GATE CONTROL CYCLE                 │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Send Ultrasonic Pulse │
              │ TRIG = HIGH (10µs)   │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Read Echo Pulse       │
              │ duration = pulseIn()  │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Calculate Distance    │
              │ cm = duration*0.034/2 │
              └───────┬───────────────┘
                      │
                      ▼
          ┌───────────────────────────┐
          │ Is Vehicle Close?         │
          │ (distance < 20cm)         │
          └───────┬───────┬───────────┘
                  │       │
               NO │       │ YES
                  │       │
                  ▼       ▼
      ┌──────────────┐  ┌─────────────┐
      │ Close Gate   │  │ Open Gate   │
      │ Servo = 0°   │  │ Servo = 90° │
      └──────┬───────┘  └──────┬──────┘
             │                 │
             └────────┬────────┘
                      │
                      ▼
          ┌───────────────────────┐
          │ Send Status to ESP32  │
          │ "GATE_OPEN|Dist:15cm"│
          └───────────┬───────────┘
                      │
                      ▼
          ┌───────────────────────┐
          │ ESP32 → Firebase      │
          │ ESP32 → Blynk         │
          └───────────────────────┘
```

### Distance Measurement Math

```
Speed of sound = 340 m/s = 0.034 cm/µs

Distance = (Time × Speed) / 2
         = (duration × 0.034) / 2  cm

Example:
  Echo pulse = 1000 µs
  Distance = (1000 × 0.034) / 2 = 17 cm
```

---

## 🌧 Feature 4: Rain Protection Cover

### Workflow Diagram

```
┌─────────────────────────────────────────────────────────┐
│                RAIN COVER CONTROL CYCLE                  │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Read Rain Sensor      │
              │ (Digital D7)         │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Is it Raining?       │
              │ (sensor == LOW)      │
              └───────┬───────┬───────┘
                      │       │
                   NO │       │ YES
                      │       │
                      ▼       ▼
          ┌──────────────┐  ┌─────────────┐
          │ Open Cover   │  │ Close Cover │
          │ Servo = 0°   │  │ Servo = 90° │
          └──────┬───────┘  └──────┬──────┘
                 │                 │
                 └────────┬────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Send Status to ESP32  │
              │ "COVER_CLOSE|Rain"   │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ ESP32 → Firebase      │
              │ ESP32 → Blynk         │
              └───────────────────────┘
```

---

## 🌡️ Feature 5: Environmental Monitoring (ESP32)

### Workflow Diagram

```
┌─────────────────────────────────────────────────────────┐
│           ENVIRONMENTAL MONITORING CYCLE                 │
│              (Every 2 seconds)                          │
└─────────────────────────────────────────────────────────┘
                          │
                ┌─────────┴─────────┐
                │                   │
                ▼                   ▼
    ┌───────────────────┐  ┌───────────────────┐
    │ Read DHT11        │  │ Read MQ Smoke     │
    │ - Temperature     │  │ Sensor            │
    │ - Humidity        │  │ (Analog GPIO34)   │
    └────────┬──────────┘  └────────┬──────────┘
             │                      │
             │                      │
             ▼                      ▼
    ┌───────────────────┐  ┌───────────────────┐
    │ Validate Data     │  │ Check Threshold   │
    │ (not NaN)         │  │ (smoke > 400?)    │
    └────────┬──────────┘  └────────┬──────────┘
             │                      │
             │                      ▼
             │          ┌───────────────────────┐
             │          │ If > 400:             │
             │          │ - Blynk notification  │
             │          │ - Firebase alert      │
             │          └───────────┬───────────┘
             │                      │
             └──────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │ Send to Cloud:        │
            │ - Firebase Database   │
            │ - Blynk App          │
            └───────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │ Update Widgets:       │
            │ - V1: Temperature     │
            │ - V6: Humidity        │
            │ - V7: Smoke Level     │
            └───────────────────────┘
```

### Smoke Detection Thresholds

| Level | Range | Status | Action |
|-------|-------|--------|--------|
| Normal | 0-200 | Safe | None |
| Warning | 200-400 | Monitor | Log to database |
| **Danger** | **400+** | **Alert** | **Send notification** |

---

## 🔐 Feature 6: Face Recognition Door Lock (ESP32-CAM)

### Complete Workflow

```
┌─────────────────────────────────────────────────────────┐
│              FACE RECOGNITION WORKFLOW                   │
└─────────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Main ESP32:           │
              │ IR Sensor Detects     │
              │ Motion at Door        │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ ESP32 sends trigger:  │
              │ GPIO13 = HIGH (100ms) │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ ESP32-CAM:            │
              │ Receives Trigger on   │
              │ GPIO13                │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Capture Image Frame   │
              │ (OV2640 camera)       │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Convert to RGB888     │
              │ (for neural network)  │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Neural Network:       │
              │ Face Detection        │
              │ (MTMNN algorithm)     │
              └───────┬───────────────┘
                      │
        ┌─────────────┴──────────────┐
        │                            │
     NO │                            │ YES
        ▼                            ▼
┌───────────────┐        ┌───────────────────┐
│ No Face Found │        │ Face(s) Detected  │
│               │        │                   │
│ Keep Locked   │        │ Face Recognition  │
│               │        │ (Compare with     │
└───────────────┘        │  enrolled faces)  │
                         └────────┬──────────┘
                                  │
                    ┌─────────────┴──────────────┐
                    │                            │
                 NO │                            │ YES
                    ▼                            ▼
        ┌────────────────────┐      ┌────────────────────┐
        │ Unknown Face       │      │ Face Matched!      │
        │ - Keep locked      │      │ - Unlock door      │
        │ - LED blinks fast  │      │ - Servo = 90°      │
        │ - Log attempt      │      │ - LED blinks slow  │
        └────────────────────┘      └─────────┬──────────┘
                                              │
                                              ▼
                                  ┌────────────────────┐
                                  │ Wait 5 seconds     │
                                  └─────────┬──────────┘
                                            │
                                            ▼
                                  ┌────────────────────┐
                                  │ Lock door          │
                                  │ Servo = 0°         │
                                  └─────────┬──────────┘
                                            │
                                            ▼
                                  ┌────────────────────┐
                                  │ Log to Firebase    │
                                  │ Send notification  │
                                  └────────────────────┘
```

### Face Recognition Algorithm Details

**Step 1: Face Detection (MTMNN)**
```
Input: RGB image (320x240)
Process:
  1. Multi-scale image pyramid
  2. Proposal Network (P-Net)
  3. Refinement Network (R-Net)
  4. Output Network (O-Net)
Output: Face bounding boxes and landmarks
```

**Step 2: Face Recognition**
```
Input: Detected face region
Process:
  1. Extract facial features (embedding)
  2. Compare with enrolled face embeddings
  3. Calculate similarity scores
  4. Threshold comparison (default: 0.6)
Output: Match ID or -1 (unknown)
```

---

## 📊 Data Flow Architecture

### Real-time Data Flow

```
┌─────────────┐
│   SENSORS   │
└──────┬──────┘
       │
       │ Analog/Digital Signals
       │
       ▼
┌─────────────────────┐
│   Arduino Uno       │
│   - Read sensors    │ ──Serial(9600)──┐
│   - Control outputs │                 │
│   - Send status     │                 │
└─────────────────────┘                 │
                                        │
┌─────────────────────┐                 │
│   ESP32 Sensors     │                 │
│   - DHT11           │                 │
│   - MQ Smoke        │                 │
│   - IR Motion       │                 │
└──────┬──────────────┘                 │
       │                                │
       │ I2C/Analog/Digital             │
       │                                │
       ▼                                ▼
┌──────────────────────────────────────────┐
│              ESP32 IoT Hub               │
│  ┌────────────────────────────────────┐  │
│  │  Data Processing & Aggregation     │  │
│  └────────────┬───────────────────────┘  │
│               │                           │
│    ┌──────────┴──────────┐               │
│    │                     │               │
│    ▼                     ▼               │
│  Firebase             Blynk              │
│  Realtime DB          Cloud              │
└──────┬─────────────────┬─────────────────┘
       │                 │
       │ HTTPS           │ WebSocket
       │                 │
       ▼                 ▼
┌────────────┐    ┌────────────┐
│  Firebase  │    │   Blynk    │
│  Console   │    │  Mobile App│
│  (Web)     │    │  (iOS/And) │
└────────────┘    └────────────┘
```

### Control Flow (User → Actuators)

```
┌────────────┐
│ Blynk App  │
│  (User)    │
└──────┬─────┘
       │
       │ Command via WebSocket
       │
       ▼
┌──────────────┐
│ Blynk Cloud  │
└──────┬───────┘
       │
       │ HTTPS
       │
       ▼
┌────────────────────┐
│ ESP32 (receives    │
│ via virtual pin)   │
└──────┬─────────────┘
       │
       │ Serial (9600)
       │
       ▼
┌────────────────────┐
│ Arduino            │
│ - Parse command    │
│ - Control actuator │
└────────────────────┘
```

---

## ⏱️ Timing & Synchronization

### Loop Cycle Times

| Module | Loop Frequency | Delay |
|--------|----------------|-------|
| Arduino | ~1Hz | 1000ms |
| ESP32 | ~10Hz | 100ms |
| ESP32-CAM | Variable | Event-driven |

### Sensor Reading Intervals

| Sensor | Reading Frequency | Notes |
|--------|-------------------|-------|
| Soil Moisture | Every 1s | Slow-changing |
| Rain Sensor | Every 1s | Instant detection |
| IR Motion | Continuous | Interrupt-driven |
| Ultrasonic | Every 1s | Distance monitoring |
| DHT11 | Every 2s | DHT limit |
| MQ Smoke | Every 2s | Warm-up needed |

### Communication Timing

```
Arduino → ESP32 (Serial):  Every 1 second
ESP32 → Firebase:          Every 2 seconds
ESP32 → Blynk:            Every 2 seconds
ESP32 → ESP32-CAM:        On event (motion detected)
ESP32-CAM → User:         On trigger
```

---

## 🔒 Error Handling & Fault Tolerance

### Arduino Error Handling

```cpp
// Sensor validation
if (distance > 400 || distance == 0) {
    // Invalid ultrasonic reading - ignore
    distance = lastValidDistance;
}

// Timeout protection
duration = pulseIn(ECHO, HIGH, 30000);  // 30ms timeout
if (duration == 0) {
    // No echo received
    distance = 999;  // Error value
}
```

### ESP32 Error Handling

```cpp
// WiFi reconnection
if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    delay(1000);
}

// Firebase retry logic
if (!Firebase.setString(fbdo, path, value)) {
    Serial.println("Retry: " + fbdo.errorReason());
    // Data lost, but system continues
}

// DHT11 validation
if (isnan(humidity) || isnan(temperature)) {
    // Use last known good values
    humidity = lastHumidity;
    temperature = lastTemperature;
}
```

### ESP32-CAM Error Handling

```cpp
// Camera capture failure
camera_fb_t * fb = esp_camera_fb_get();
if (!fb) {
    Serial.println("Camera capture failed - retry");
    esp_camera_fb_return(fb);
    return;
}

// Face detection failure
box_array_t *faces = face_detect(image_matrix, &mtmn_config);
if (!faces || faces->len == 0) {
    // No face detected - normal operation
    Serial.println("No face detected");
    // Door remains locked
}

// Memory allocation failure
dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, w, h, 3);
if (!image_matrix) {
    Serial.println("Memory allocation failed");
    esp_camera_fb_return(fb);
    return;
}
```

---

## 🎯 Performance Optimization

### Power Consumption Optimization

**Arduino:**
- Use `delay()` instead of busy loops
- Put unused peripherals to sleep
- Disable unused pins

**ESP32:**
- Use deep sleep when idle (future enhancement)
- Reduce WiFi transmit power if close to router
- Batch Firebase updates

**ESP32-CAM:**
- Keep camera off until triggered
- Use lower resolution for face detection
- Optimize frame buffer count

### Memory Optimization

**ESP32-CAM Critical:**
```cpp
// Use PSRAM for large buffers
if (psramFound()) {
    config.fb_count = 2;  // Double buffering
} else {
    config.fb_count = 1;  // Single buffer
}

// Free memory immediately after use
dl_matrix3du_free(image_matrix);
esp_camera_fb_return(fb);
free(faces);
```

---

## 📈 System Scalability

### Adding New Sensors

1. **Arduino Side:**
   ```cpp
   #define NEW_SENSOR_PIN A1
   void loop() {
       int value = analogRead(NEW_SENSOR_PIN);
       Serial.println("NEW_SENSOR:" + String(value));
   }
   ```

2. **ESP32 Side:**
   ```cpp
   if (msg.startsWith("NEW_SENSOR:")) {
       int value = msg.substring(11).toInt();
       Firebase.setInt(fbdo, "/home/newSensor", value);
       Blynk.virtualWrite(V8, value);
   }
   ```

3. **Blynk Side:**
   - Add new datastream (V8)
   - Add widget to dashboard

### Adding New Actuators

Follow same pattern as existing pump/light control.

---

## 🔍 Debugging & Monitoring

### Serial Monitor Output Interpretation

**Arduino Normal Operation:**
```
PUMP_OFF|Soil_Moist
LIGHT_OFF
GATE_CLOSE
COVER_OPEN
---
```

**ESP32 Normal Operation:**
```
[DHT11] Temp: 28.3°C | Humidity: 65.5%
[Smoke] Level: 145
[Arduino] Received: PUMP_OFF
[Firebase] Arduino data uploaded
```

**ESP32-CAM Normal Operation:**
```
[TRIGGER] Face recognition activated!
[Camera] Frame captured
[Processing] Detecting faces...
[Detected] 1 face(s) found
✅ FACE MATCHED!
Door closed and locked
```

---

## 📊 System State Machine

### Overall System States

```
┌──────────────┐
│ INITIALIZING │
└──────┬───────┘
       │
       ▼
┌──────────────┐      Network
│   STARTING   │─────► Error ──┐
└──────┬───────┘                │
       │                        │
       ▼                        │
┌──────────────┐                │
│    READY     │◄───────────────┘
└──────┬───────┘
       │
       ├──► MONITORING ◄──┐
       │         │        │
       │         ▼        │
       └──► CONTROLLING ──┘
```

---

**This document provides the complete technical workflow of the system. For implementation details, refer to the code files.**
