/*
 * ========================================
 * SMART HOME - ESP32 IoT HUB
 * ========================================
 * 
 * Description: IoT gateway connecting local automation to cloud
 * Board: ESP32 DevKit
 * 
 * Features:
 * - WiFi connectivity
 * - Firebase Realtime Database integration
 * - Blynk mobile app integration
 * - Environmental monitoring (DHT11)
 * - Smoke/gas detection
 * - Security monitoring (IR motion)
 * - ESP32-CAM trigger control
 * - Arduino data bridge
 * 
 * Author: Your Name
 * Date: February 2026
 * Version: 1.0
 * 
 * ========================================
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// ========== CREDENTIALS (REPLACE WITH YOUR OWN) ==========
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your_firebase_database_secret"
#define BLYNK_AUTH "your_blynk_auth_token"

// ========== PIN DEFINITIONS ==========
#define IR_MAIN 27        // Main door IR sensor
#define CAM_TRIGGER 13    // ESP32-CAM trigger pin
#define SMOKE 34          // MQ-2/MQ-135 smoke sensor (analog)
#define DHTPIN 14         // DHT11 temperature & humidity sensor

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ========== FIREBASE & BLYNK ==========
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

// ========== TIMING VARIABLES ==========
unsigned long lastSensorRead = 0;
const long sensorInterval = 2000;  // Read sensors every 2 seconds

bool irTriggered = false;
unsigned long irTriggerTime = 0;
const long irCooldown = 5000;  // 5 second cooldown between triggers

void setup() {
  Serial.begin(9600);  // Communication with Arduino
  
  // ========== PIN SETUP ==========
  pinMode(IR_MAIN, INPUT);
  pinMode(CAM_TRIGGER, OUTPUT);
  pinMode(SMOKE, INPUT);
  
  digitalWrite(CAM_TRIGGER, LOW);  // Initialize camera trigger to LOW
  
  dht.begin();
  
  // ========================================
  // WIFI CONNECTION
  // ========================================
  Serial.println("\n[WiFi] Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] Connection FAILED!");
    return;
  }
  
  // ========================================
  // FIREBASE SETUP
  // ========================================
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("[Firebase] Connected!");
  
  // ========================================
  // BLYNK SETUP
  // ========================================
  Blynk.config(BLYNK_AUTH);
  Blynk.connect();
  
  if (Blynk.connected()) {
    Serial.println("[Blynk] Connected!");
  } else {
    Serial.println("[Blynk] Connection FAILED!");
  }
  
  // Initial status
  Firebase.setString(fbdo, "/home/status", "System Online");
  Blynk.virtualWrite(V5, "System Online");
  
  Serial.println("\n========== SYSTEM READY ==========\n");
}

void loop() {
  Blynk.run();
  
  // ========================================
  // RECEIVE DATA FROM ARDUINO
  // ========================================
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();  // Remove whitespace
    
    if (msg.length() > 0) {
      Serial.print("[Arduino] Received: ");
      Serial.println(msg);
      
      // Send to Firebase
      if (Firebase.setString(fbdo, "/home/status", msg)) {
        Serial.println("[Firebase] Arduino data uploaded");
      } else {
        Serial.println("[Firebase] Upload failed: " + fbdo.errorReason());
      }
      
      // Send to Blynk
      Blynk.virtualWrite(V5, msg);
    }
  }
  
  // ========================================
  // READ SENSORS (Every 2 seconds)
  // ========================================
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;
    
    // ========== DHT11 HUMIDITY & TEMPERATURE ==========
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    // ========== SMOKE SENSOR ==========
    int smokeLevel = analogRead(SMOKE);
    
    // Validate DHT readings
    if (!isnan(humidity) && !isnan(temperature)) {
      // Send to Firebase
      Firebase.setFloat(fbdo, "/home/humidity", humidity);
      Firebase.setFloat(fbdo, "/home/temperature", temperature);
      
      // Send to Blynk
      Blynk.virtualWrite(V6, humidity);
      Blynk.virtualWrite(V1, temperature);
      
      Serial.print("[DHT11] Temp: ");
      Serial.print(temperature);
      Serial.print("°C | Humidity: ");
      Serial.print(humidity);
      Serial.println("%");
    } else {
      Serial.println("[DHT11] Read failed!");
    }
    
    // Send smoke data
    Firebase.setInt(fbdo, "/home/smoke", smokeLevel);
    Blynk.virtualWrite(V7, smokeLevel);
    
    Serial.print("[Smoke] Level: ");
    Serial.println(smokeLevel);
    
    // ========== SMOKE ALERT ==========
    if (smokeLevel > 400) {  // Threshold for smoke detection
      Blynk.logEvent("smoke_alert", "High smoke detected!");
      Firebase.setString(fbdo, "/home/alerts", "SMOKE_DETECTED");
      Serial.println("[ALERT] SMOKE DETECTED!");
    }
  }
  
  // ========================================
  // IR SENSOR → TRIGGER ESP32-CAM
  // ========================================
  if (digitalRead(IR_MAIN) == LOW) {  // Motion detected
    // Cooldown check to prevent spam
    if (!irTriggered || (currentMillis - irTriggerTime > irCooldown)) {
      Serial.println("[IR] Motion detected at main door!");
      
      // Trigger ESP32-CAM
      digitalWrite(CAM_TRIGGER, HIGH);
      delay(100);  // Short pulse
      digitalWrite(CAM_TRIGGER, LOW);
      
      // Send notifications
      Blynk.logEvent("intruder", "Person detected at door");
      Firebase.setString(fbdo, "/home/camera", "TRIGGERED");
      Firebase.setString(fbdo, "/home/lastMotion", String(currentMillis));
      
      irTriggered = true;
      irTriggerTime = currentMillis;
      
      Serial.println("[Camera] Triggered!");
    }
  } else {
    // Reset trigger flag when no motion
    if (irTriggered && (currentMillis - irTriggerTime > irCooldown)) {
      irTriggered = false;
    }
  }
  
  delay(100);  // Small delay for stability
}

/*
 * ========================================
 * BLYNK VIRTUAL PIN MAPPING
 * ========================================
 * 
 * V1  → Temperature (°C)
 * V5  → Arduino Status (Terminal/Label)
 * V6  → Humidity (%)
 * V7  → Smoke Level (Gauge)
 * 
 * BLYNK EVENTS:
 * - intruder: Motion detected at door
 * - smoke_alert: High smoke level detected
 * 
 * ========================================
 */

/*
 * ========================================
 * FIREBASE DATABASE STRUCTURE
 * ========================================
 * 
 * /home/
 *   ├── status: "PUMP_ON|Soil:650"
 *   ├── humidity: 65.5
 *   ├── temperature: 28.3
 *   ├── smoke: 145
 *   ├── camera: "TRIGGERED"
 *   ├── lastMotion: "1234567890"
 *   └── alerts: "SMOKE_DETECTED"
 * 
 * ========================================
 */

/*
 * ========================================
 * WIRING REFERENCE
 * ========================================
 * 
 * ESP32 CONNECTIONS:
 * - DHT11: VCC→3.3V, GND→GND, DATA→GPIO14
 * - MQ Smoke: VCC→5V, GND→GND, AO→GPIO34
 * - IR Motion: VCC→5V, GND→GND, OUT→GPIO27
 * - ESP32-CAM Trigger: GPIO13 → ESP32-CAM GPIO13
 * - Arduino Serial: RX2(GPIO16)←Arduino TX, TX2(GPIO17)→Arduino RX
 * 
 * POWER:
 * - ESP32: 5V via USB or VIN pin
 * - All sensors share common GND with ESP32
 * 
 * ========================================
 */

/*
 * ========================================
 * SETUP INSTRUCTIONS
 * ========================================
 * 
 * 1. FIREBASE SETUP:
 *    - Go to console.firebase.google.com
 *    - Create new project
 *    - Enable Realtime Database
 *    - Get Database URL (without https://)
 *    - Get Database Secret: Project Settings → Service Accounts
 * 
 * 2. BLYNK SETUP:
 *    - Download Blynk app
 *    - Create new project (ESP32)
 *    - Copy Auth Token from email
 *    - Add widgets:
 *      • Value Display (V1) - Temperature
 *      • Terminal/Label (V5) - Arduino Status
 *      • Value Display (V6) - Humidity
 *      • Gauge (V7) - Smoke Level
 *    - Create Events:
 *      • intruder
 *      • smoke_alert
 * 
 * 3. WIFI:
 *    - Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
 *    - Update WIFI_SSID and WIFI_PASS
 * 
 * ========================================
 */

/*
 * ========================================
 * TROUBLESHOOTING
 * ========================================
 * 
 * Issue: WiFi not connecting
 * - Verify SSID and password
 * - Check 2.4GHz WiFi (not 5GHz)
 * - Move closer to router
 * 
 * Issue: Firebase connection failed
 * - Check Firebase host (without https://)
 * - Verify database secret key
 * - Check database rules (allow read/write)
 * 
 * Issue: Blynk not connecting
 * - Verify Auth Token
 * - Check Blynk server status
 * - Ensure project is not in STOP mode
 * 
 * Issue: DHT11 returning NaN
 * - Check wiring (VCC, GND, Data)
 * - Add 10kΩ pull-up resistor
 * - Wait 2 seconds between readings
 * 
 * Issue: Serial data not received from Arduino
 * - Check TX/RX connections (crossover)
 * - Verify common GND
 * - Check baud rate (9600)
 * 
 * ========================================
 */
