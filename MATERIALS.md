# Bill of Materials (BOM)

Complete component list with specifications, quantities for the Smart Home Automation System.

---

## 💰 Cost Summary

| Category |
|----------|
| Microcontrollers |
| Sensors |
| Actuators |
| Power Supplies |
| Cables & Connectors |
| Miscellaneous |


---

## 🎛️ Module 1: Arduino Uno - Local Automation

### Microcontroller

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| Arduino Uno R3 | ATmega328P, USB | 1 | Official or compatible |

### Sensors

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| Soil Moisture Sensor | Analog output | 1 | Capacitive or resistive |
| Rain Sensor | Digital output | 1 | FC-37 or similar |
| IR Motion Sensor | PIR HC-SR501 | 1 | Adjustable sensitivity |
| Ultrasonic Sensor | HC-SR04 | 1 | 2cm-4m range |


### Actuators

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| Servo Motor | SG90, 9g micro | 2 | For gate & rain cover |
| Water Pump | 3-6V DC submersible | 1 | 120L/H or similar |
| Relay Module | 3-channel, 5V | 1 | 10A rating minimum |
| Light Bulb | LED 5W or compatible | 1 | With holder |


### Power & Cables

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| Power Adapter | 9-12V DC, 1A | 1 | Barrel jack type |
| Jumper Wires | M-M, M-F, F-F | 40pcs | Various colors |
| Breadboard | 830 point | 1 | Half-size or full |


---

## 📡 Module 2: ESP32 - IoT Gateway

### Microcontroller

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| ESP32 DevKit | ESP32-WROOM-32 | 1 | Any variant (30/38 pin) |

### Sensors

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| DHT11 Sensor | Temp & Humidity | 1 | 3-pin module preferred |
| MQ-2 Smoke Sensor | Gas sensor module | 1 | Or MQ-135 |
| IR Motion Sensor | PIR HC-SR501 | 1 | For door monitoring |


### Power & Cables

| Item | Specs | Qty |  Notes |
|------|-------|-----|-------|
| USB Cable | Micro-USB, data | 1 | For programming & power |
| Power Adapter | 5V 2A USB | 1 | Quality adapter |
| Jumper Wires | M-M, M-F | 20pcs | Assorted |
| Breadboard | 400 point | 1 | Mini breadboard |



---

## 📷 Module 3: ESP32-CAM - Face Recognition

### Microcontroller & Camera

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| ESP32-CAM | AI-Thinker + OV2640 | 1 | With camera included |

### Actuator

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| Servo Motor | SG90, 9g micro | 1 | Door lock mechanism |

### Programming & Power

| Item | Specs | Qty | Notes |
|------|-------|-----|-------|
| FTDI Programmer | USB-TTL, 5V/3.3V | 1 | CP2102 or FT232RL |
| Power Adapter | 5V 2A (crucial!) | 1 |High quality required |
| Jumper Wires | M-F | 10pcs | Female-to-female |

---

## 🔌 Additional Components


| Item | Specs | Qty | Purpose |
|------|-------|-----|---------|
| Capacitors | 100µF, 470µF electrolytic | 5 | Power stabilization |
| Resistors | 10kΩ, 220Ω | 10 | Pull-ups, LEDs |
| LEDs | 5mm, various colors | 5 | Status indicators |
| Heat Shrink Tubing | Assorted sizes | 1 set |Wire protection |
| Enclosures | Plastic project boxes | 3 | Weather protection |
| Standoffs | M3 brass, 10mm | 20 |  Board mounting |
| Screws & Nuts | M3 assortment | 1 set | Assembly |
| Velcro Tape | Adhesive back | 1m | Cable management |



## 📦 Starter Kit Option

Instead of buying components individually, consider Arduino/ESP32 starter kits:

### Arduino Starter Kit
Includes:
- Arduino Uno
- Breadboard
- Jumper wires
- LEDs, resistors
- Various sensors
- USB cable

### ESP32 Development Kit
Includes:
- ESP32 board
- Breadboard
- Sensors (DHT11, ultrasonic, etc.)
- LEDs and components
- Jumper wires

**Pros:**
- Lower total cost
- Includes extras for learning
- All compatible components

**Cons:**
- May include unnecessary items
- Specific sensors might not be included
- Quality can vary

---

## 🔍 Component Specifications

### Detailed Sensor Specs

#### Soil Moisture Sensor
```
Type: Capacitive (recommended) or Resistive
Voltage: 3.3V-5V
Output: Analog (0-1023 on Arduino)
Probe Material: Anti-corrosion coating
Typical Use: Direct soil insertion
```

#### Rain Sensor
```
Type: Resistive rain detection
Voltage: 3.3V-5V
Output: Digital (HIGH/LOW) and Analog
Detection Area: ~5cm x 4cm
Sensitivity: Adjustable via potentiometer
```

#### HC-SR501 PIR Motion Sensor
```
Voltage: 5V-20V (typically use 5V)
Detection Range: 3-7 meters
Detection Angle: 120°
Output: Digital (HIGH when motion)
Adjustments: Sensitivity and time delay
Trigger Modes: Repeatable/Non-repeatable
```

#### HC-SR04 Ultrasonic
```
Voltage: 5V
Range: 2cm - 400cm
Accuracy: ±3mm
Frequency: 40kHz
Trigger Input: 10µs HIGH pulse
Echo Output: Pulse width = distance
```

#### DHT11 Temperature & Humidity
```
Voltage: 3.3V-5V
Temperature Range: 0-50°C (±2°C)
Humidity Range: 20-90% RH (±5%)
Sampling Rate: 1Hz (once per second)
Pin Configuration: VCC, DATA, NC, GND
Pull-up Resistor: 10kΩ recommended
```

#### MQ-2 Smoke/Gas Sensor
```
Voltage: 5V
Detection: LPG, Propane, Methane, Smoke
Concentration: 300-10,000ppm
Warm-up Time: 20-30 seconds
Output: Analog (varies with concentration)
Note: Requires 24-48hr burn-in for accuracy
```

### Detailed Actuator Specs

#### SG90 Servo Motor
```
Voltage: 4.8V-6V
Torque: 1.8kg/cm (4.8V)
Speed: 0.1s/60° (4.8V)
Rotation: 180° (typically 0-180)
Control: PWM (50Hz)
Current: 100-250mA (no load to stall)
Weight: 9g
```

#### Water Pump
```
Voltage: 3-6V DC
Flow Rate: 80-120 L/hr
Max Head: 40-80cm
Power: 2-3W
Current: ~500mA
Type: Submersible
Tube Diameter: 4-7mm
```

#### Relay Module (3-Channel)
```
Voltage: 5V control, up to 250VAC/30VDC load
Current Rating: 10A per channel
Trigger: LOW or HIGH (check module type)
Indicator: LED per channel
Isolation: Optocoupler
Switching Time: <10ms
```

---

## ⚡ Power Supply Requirements

### Voltage & Current Specifications

| Component | Voltage | Current (Typical) | Current (Peak) |
|-----------|---------|-------------------|----------------|
| Arduino Uno | 7-12V (9V ideal) | ~50mA | ~200mA |
| ESP32 | 5V | ~160mA | ~500mA |
| ESP32-CAM | 5V | ~180mA | ~800mA |
| Servo (SG90) | 4.8-6V | ~10mA | ~250mA |
| DHT11 | 3.3-5V | ~1mA | ~2.5mA |
| MQ-2 | 5V | ~150mA | ~200mA |
| HC-SR04 | 5V | ~15mA | ~20mA |
| PIR Sensor | 5V | ~50µA | ~65mA |
| Water Pump | 3-6V | ~200mA | ~500mA |
| Relay Module | 5V | ~15mA | ~100mA |

### Power Distribution Strategy

#### Option 1: Separate Power Supplies (Recommended)
```
Arduino:    9V 1A adapter → Arduino barrel jack
ESP32:      5V 2A adapter → USB or VIN pin
ESP32-CAM:  5V 2A adapter → Dedicated supply
Servos:     5V 2A adapter → Shared with pump/relay
            (common GND with control boards)
```

#### Option 2: Single High-Current Supply
```
12V 5A Power Supply
    ├→ Arduino: 12V input (voltage regulator on board)
    ├→ Buck converter 1: 12V → 5V 3A (ESP32 + ESP32-CAM)
    ├→ Buck converter 2: 12V → 5V 2A (Servos + pump)
    └→ Common GND (all modules)
```

### Recommended Power Supplies

| Type | Specs | Use Case |
|------|-------|----------|
| Wall Adapter | 9V 1A | Arduino Uno |
| USB Charger | 5V 2A | ESP32 |
| USB Charger | 5V 2A | ESP32-CAM |
| Power Strip | Multi-outlet | All supplies |


## 📋 Shopping List Template

Copy this to organize your purchase:

```
☐ MICROCONTROLLERS
  ☐ Arduino Uno R3 (1x)
  ☐ ESP32 DevKit (1x)
  ☐ ESP32-CAM with camera (1x)

☐ SENSORS
  ☐ Soil moisture sensor (1x)
  ☐ Rain sensor (1x)
  ☐ PIR motion sensor (2x)
  ☐ HC-SR04 ultrasonic (1x)
  ☐ DHT11 temp/humidity (1x)
  ☐ MQ-2 smoke sensor (1x)

☐ ACTUATORS
  ☐ SG90 servo motors (3x)
  ☐ Water pump (1x)
  ☐ Relay module 3-channel (1x)
  ☐ LED light bulb (1x)

☐ POWER SUPPLIES
  ☐ 9V 1A adapter (1x)
  ☐ 5V 2A adapters (2x)

☐ PROGRAMMING
  ☐ FTDI programmer (1x)
  ☐ USB cables (2x)

☐ CONNECTIONS
  ☐ Jumper wires M-M (40x)
  ☐ Jumper wires M-F (20x)
  ☐ Jumper wires F-F (10x)
  ☐ Breadboards (2x)

☐ EXTRAS
  ☐ Capacitors (assorted)
  ☐ Resistors (assorted)
  ☐ LEDs (5x)
  ☐ Enclosures (3x)
```

## 🔧 Tools Required (Not in BOM)

You'll also need these common tools:

| Tool | Purpose |
|------|---------|
| Soldering iron | Optional connections |
| Wire strippers | Prepare wires |
| Screwdrivers | Assembly |
| Multimeter | Testing/debugging |
| Hot glue gun | Securing components |
| Helping hands | Soldering aid |
| Side cutters | Wire cutting |


---

## 📊 Budget Versions

### Minimal Version 
- Arduino Uno only (no ESP32/ESP32-CAM)
- 2 sensors (soil + ultrasonic)
- 1 servo + 1 pump
- Basic functionality, no IoT

### Mid-Range Version
- Arduino Uno + ESP32 (no ESP32-CAM)
- All sensors
- All actuators
- IoT features, no face recognition

### Full Version 
- All modules
- All sensors
- All actuators
- Complete system as designed

### Premium Version 
- Quality branded components
- Extra sensors for redundancy
- Professional enclosures
- Better power management

---

## ✅ Purchase Checklist

Before ordering, verify:

```
☐ Component quantity matches your needs
☐ Voltage ratings are correct (3.3V vs 5V)
☐ Current ratings adequate for power supplies
☐ Connector types match (male/female)
☐ Cables are data-capable (not charge-only)
☐ Shipping cost included in budget
☐ Estimated delivery time acceptable
☐ Return policy understood
☐ Seller rating checked (>95% positive)
☐ Product reviews read
```

---

## 📦 Storage & Organization

Once you receive components:

**Organize by Type:**
- Small containers for resistors/capacitors
- Labeled bags for sensors
- Separate box for each module
- Cable management for wires

**Label Everything:**
- Component values
- Purchase date
- Working/tested status

**Keep Documentation:**
- Datasheets (download and print)
- Purchase receipts
- Warranty information

---

