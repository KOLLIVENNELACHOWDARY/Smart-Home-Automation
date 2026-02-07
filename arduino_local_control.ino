/*
 * ========================================
 * SMART HOME - ARDUINO LOCAL CONTROL
 * ========================================
 * 
 * Description: Local automation controller for smart home
 * Board: Arduino Uno
 * 
 * Features:
 * - Automatic irrigation based on soil moisture
 * - Motion-activated room lighting
 * - Automatic parking gate control
 * - Rain-protective cover automation
 * 
 * Author: Your Name
 * Date: February 2026
 * Version: 1.0
 * 
 * ========================================
 */

#include <Servo.h>

// ========== PIN DEFINITIONS ==========
#define SOIL A0          // Soil moisture sensor (analog)
#define RAIN 7           // Rain sensor (digital)
#define IR_ROOM 6        // IR motion sensor for room
#define TRIG 9           // Ultrasonic sensor trigger
#define ECHO 10          // Ultrasonic sensor echo
#define PUMP 3           // Water pump relay control
#define LIGHT 4          // Room light relay control
#define FAN 11           // Fan relay control (reserved)

// Servo motors
Servo gateServo;         // Parking gate servo (D5)
Servo rainServo;         // Rain cover servo (D8)

// Variables
long duration;
int distance;

void setup() {
  // ========== SERIAL COMMUNICATION ==========
  Serial.begin(9600);  // Communication with ESP32
  
  // ========== PIN MODES ==========
  pinMode(RAIN, INPUT);
  pinMode(IR_ROOM, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  // ========== SERVO SETUP ==========
  gateServo.attach(5);
  rainServo.attach(8);
  
  // ========== INITIALIZE OUTPUTS ==========
  digitalWrite(PUMP, LOW);
  digitalWrite(LIGHT, LOW);
  digitalWrite(FAN, LOW);
  gateServo.write(0);    // Gate closed
  rainServo.write(0);    // Cover open
  
  Serial.println("SYSTEM_INIT");
  delay(1000);
}

void loop() {
  // ========================================
  // 🌱 IRRIGATION SYSTEM
  // ========================================
  int soil = analogRead(SOIL);
  int rain = digitalRead(RAIN);
  
  // Logic: Water plants when soil is dry AND no rain detected
  // Soil sensor: Higher value = drier soil (typically > 600 is dry)
  // Rain sensor: HIGH = no rain, LOW = rain detected
  if (soil > 600 && rain == HIGH) {
    digitalWrite(PUMP, HIGH);
    Serial.print("PUMP_ON|Soil:");
    Serial.println(soil);
  } else {
    digitalWrite(PUMP, LOW);
    if (soil <= 600) {
      Serial.println("PUMP_OFF|Soil_Moist");
    } else if (rain == LOW) {
      Serial.println("PUMP_OFF|Rain_Detected");
    } else {
      Serial.println("PUMP_OFF");
    }
  }
  
  // ========================================
  // 💡 ROOM LIGHT (Motion Detection)
  // ========================================
  // IR sensor: LOW = motion detected, HIGH = no motion
  if (digitalRead(IR_ROOM) == LOW) {
    digitalWrite(LIGHT, HIGH);
    Serial.println("LIGHT_ON|Motion");
  } else {
    digitalWrite(LIGHT, LOW);
    Serial.println("LIGHT_OFF");
  }
  
  // ========================================
  // 🚗 PARKING GATE (Ultrasonic Distance)
  // ========================================
  // Trigger ultrasonic pulse
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // Read echo and calculate distance
  duration = pulseIn(ECHO, HIGH, 30000);  // 30ms timeout
  distance = duration * 0.034 / 2;        // Convert to cm
  
  // Open gate if vehicle within 20cm (and valid reading)
  if (distance < 20 && distance > 0) {
    gateServo.write(90);  // Open position
    Serial.print("GATE_OPEN|Dist:");
    Serial.print(distance);
    Serial.println("cm");
  } else {
    gateServo.write(0);   // Closed position
    Serial.println("GATE_CLOSE");
  }
  
  // ========================================
  // 🌧 RAIN COVER
  // ========================================
  // Rain sensor: LOW = rain detected, HIGH = no rain
  if (rain == LOW) {
    rainServo.write(90);  // Close cover
    Serial.println("COVER_CLOSE|Rain");
  } else {
    rainServo.write(0);   // Open cover
    Serial.println("COVER_OPEN");
  }
  
  // ========================================
  Serial.println("---");  // Message separator
  delay(1000);  // 1 second cycle time
}

/*
 * ========================================
 * SERIAL OUTPUT FORMAT
 * ========================================
 * 
 * The Arduino sends status messages to ESP32 via Serial.
 * Format: STATUS|Details
 * 
 * Examples:
 * - PUMP_ON|Soil:650
 * - LIGHT_ON|Motion
 * - GATE_OPEN|Dist:15cm
 * - COVER_CLOSE|Rain
 * 
 * ESP32 receives these and forwards to Firebase/Blynk
 * 
 * ========================================
 */

/*
 * ========================================
 * WIRING REFERENCE
 * ========================================
 * 
 * SENSORS:
 * - Soil Moisture: VCC→5V, GND→GND, AO→A0
 * - Rain Sensor: VCC→5V, GND→GND, DO→D7
 * - IR Motion: VCC→5V, GND→GND, OUT→D6
 * - Ultrasonic: VCC→5V, GND→GND, TRIG→D9, ECHO→D10
 * 
 * ACTUATORS:
 * - Water Pump: Via Relay IN1 (Arduino D3)
 * - Room Light: Via Relay IN2 (Arduino D4)
 * - Fan: Via Relay IN3 (Arduino D11) [Reserved]
 * - Gate Servo: Signal→D5, VCC→5V, GND→GND
 * - Rain Servo: Signal→D8, VCC→5V, GND→GND
 * 
 * RELAY MODULE:
 * - VCC→5V, GND→GND
 * - IN1→D3, IN2→D4, IN3→D11
 * 
 * SERIAL TO ESP32:
 * - Arduino TX(D1) → ESP32 RX2(GPIO16)
 * - Arduino RX(D0) → ESP32 TX2(GPIO17)
 * - GND → GND (common ground)
 * 
 * ========================================
 */

/*
 * ========================================
 * TROUBLESHOOTING
 * ========================================
 * 
 * Issue: Pump not working
 * - Check relay wiring and power
 * - Test relay with separate signal
 * - Verify pump power supply
 * 
 * Issue: Servo jittering
 * - Use external 5V supply for servos
 * - Add 100µF capacitor across servo power
 * 
 * Issue: Ultrasonic reading 0
 * - Check TRIG/ECHO wiring
 * - Verify 5V power
 * - Test with simple sketch
 * 
 * Issue: Soil sensor always dry/wet
 * - Calibrate sensor in dry and wet soil
 * - Adjust threshold value (600)
 * 
 * Issue: IR sensor always triggering
 * - Adjust sensitivity potentiometer
 * - Check for heat sources
 * - Verify 5V power
 * 
 * ========================================
 */
