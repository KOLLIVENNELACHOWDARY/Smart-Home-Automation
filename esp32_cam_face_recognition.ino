/*
 * ========================================
 * SMART HOME - ESP32-CAM FACE RECOGNITION
 * ========================================
 * 
 * Description: AI-powered face recognition door lock
 * Board: ESP32-CAM (AI-Thinker)
 * 
 * Features:
 * - Real-time face detection using neural network
 * - Face enrollment via web interface
 * - Secure door lock control
 * - External trigger from main ESP32
 * - LED status indicators
 * - Live camera streaming
 * 
 * Author: Your Name
 * Date: February 2026
 * Version: 1.0
 * 
 * ========================================
 */

#include "esp_camera.h"
#include <WiFi.h>
#include <ESP32Servo.h>
#include "fd_forward.h"
#include "fr_forward.h"

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// ========== WIFI CREDENTIALS ==========
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ========== PIN DEFINITIONS ==========
#define SERVO_PIN 12       // Door lock servo
#define TRIGGER_PIN 13     // External trigger from main ESP32
#define LED_PIN 33         // Built-in LED (GPIO 33 on AI-Thinker)

// ========== SERVO CONTROL ==========
Servo doorServo;

// ========== FACE RECOGNITION VARIABLES ==========
static mtmn_config_t mtmn_config = {0};
static face_id_list id_list = {0};

// ========== TIMING AND STATE ==========
bool recognitionActive = false;
unsigned long lastTriggerTime = 0;
const long triggerCooldown = 3000;  // 3 seconds between scans

// External function from example
void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.println("\n========== ESP32-CAM Face Recognition Door Lock ==========");
  
  // ========================================
  // PIN SETUP
  // ========================================
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Servo setup
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);  // Locked position
  Serial.println("[Servo] Initialized - Door LOCKED");
  
  // ========================================
  // CAMERA CONFIGURATION
  // ========================================
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Frame size optimization for face recognition
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;  // 320x240 - good for face detection
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[ERROR] Camera init failed: 0x%x\n", err);
    return;
  }
  Serial.println("[Camera] Initialized successfully");
  
  // ========================================
  // CAMERA SETTINGS OPTIMIZATION
  // ========================================
  sensor_t * s = esp_camera_sensor_get();
  if (s != NULL) {
    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 = No Effect
    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);        // 0 to 4
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);       // -2 to 2
    s->set_aec_value(s, 300);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);       // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
  }
  
  // ========================================
  // WIFI CONNECTION
  // ========================================
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 30) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Start web server for face enrollment
    startCameraServer();
    Serial.println("\n========== CAMERA WEB SERVER STARTED ==========");
    Serial.print("Visit: http://");
    Serial.println(WiFi.localIP());
    Serial.println("Use the web interface to enroll faces");
  } else {
    Serial.println("\n[WiFi] Connection FAILED - Running offline mode");
  }
  
  // ========================================
  // FACE RECOGNITION INIT
  // ========================================
  mtmn_config = mtmn_init_config();
  face_id_init(&id_list, FACE_ID_SAVE_NUMBER, ENROLL_CONFIRM_TIMES);
  Serial.println("[Face Recognition] Initialized");
  
  Serial.println("\n========== SYSTEM READY ==========");
  Serial.println("Waiting for trigger signal on GPIO 13...\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // ========================================
  // TRIGGER DETECTION
  // ========================================
  // Only run face recognition when triggered by external ESP32
  if (digitalRead(TRIGGER_PIN) == HIGH && !recognitionActive) {
    // Cooldown check to prevent rapid re-triggers
    if (currentMillis - lastTriggerTime > triggerCooldown) {
      Serial.println("\n[TRIGGER] Face recognition activated!");
      digitalWrite(LED_PIN, HIGH);  // Turn on LED during recognition
      
      recognitionActive = true;
      lastTriggerTime = currentMillis;
      
      // ========================================
      // CAPTURE FRAME
      // ========================================
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("[ERROR] Camera capture failed");
        digitalWrite(LED_PIN, LOW);
        recognitionActive = false;
        return;
      }
      
      Serial.println("[Camera] Frame captured");
      
      // ========================================
      // CONVERT TO RGB888
      // ========================================
      dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
      if (!image_matrix) {
        Serial.println("[ERROR] Memory allocation failed");
        esp_camera_fb_return(fb);
        digitalWrite(LED_PIN, LOW);
        recognitionActive = false;
        return;
      }
      
      bool converted = fmt2rgb888(fb->buf, fb->len, fb->format, image_matrix->item);
      if (!converted) {
        Serial.println("[ERROR] Image conversion failed");
        dl_matrix3du_free(image_matrix);
        esp_camera_fb_return(fb);
        digitalWrite(LED_PIN, LOW);
        recognitionActive = false;
        return;
      }
      
      // ========================================
      // DETECT FACES
      // ========================================
      Serial.println("[Processing] Detecting faces...");
      box_array_t *faces = face_detect(image_matrix, &mtmn_config);
      
      if (faces && faces->len > 0) {
        Serial.printf("[Detected] %d face(s) found\n", faces->len);
        
        // ========================================
        // RECOGNIZE FACE
        // ========================================
        int matched_id = recognize_face(&id_list, image_matrix, faces);
        
        if (matched_id >= 0) {
          // ✅ AUTHORIZED - UNLOCK DOOR
          Serial.println("\n✅ FACE MATCHED!");
          Serial.printf("Recognized ID: %d\n", matched_id);
          Serial.println("Opening door...");
          
          doorServo.write(90);  // Unlock position
          
          // Blink LED to indicate success
          for (int i = 0; i < 3; i++) {
            digitalWrite(LED_PIN, LOW);
            delay(200);
            digitalWrite(LED_PIN, HIGH);
            delay(200);
          }
          
          delay(5000);  // Keep door open for 5 seconds
          
          doorServo.write(0);  // Lock position
          Serial.println("Door closed and locked\n");
          
        } else {
          // ❌ UNAUTHORIZED - KEEP LOCKED
          Serial.println("\n❌ UNKNOWN FACE - ACCESS DENIED");
          
          // Fast blink to indicate rejection
          for (int i = 0; i < 6; i++) {
            digitalWrite(LED_PIN, LOW);
            delay(100);
            digitalWrite(LED_PIN, HIGH);
            delay(100);
          }
        }
        
        // Free face detection memory
        free(faces->score);
        free(faces->box);
        free(faces->landmark);
        free(faces);
        
      } else {
        Serial.println("[Result] No face detected");
      }
      
      // ========================================
      // CLEANUP
      // ========================================
      dl_matrix3du_free(image_matrix);
      esp_camera_fb_return(fb);
      digitalWrite(LED_PIN, LOW);
      recognitionActive = false;
      
      Serial.println("Ready for next trigger\n");
    }
  }
  
  delay(100);  // Small delay for stability
}

/*
 * ========================================
 * WEB INTERFACE USAGE
 * ========================================
 * 
 * Access the ESP32-CAM web interface to enroll faces:
 * 
 * 1. Find IP address from Serial Monitor
 * 2. Open browser: http://[IP_ADDRESS]
 * 3. Click "Face Enrollment" tab
 * 4. Position face in camera view
 * 5. Click "Enroll" button
 * 6. Repeat 5-7 times from different angles
 * 7. Face ID saved in memory
 * 
 * TIPS:
 * - Good lighting is essential
 * - Face camera directly
 * - Distance: 30-100cm from camera
 * - Enroll from multiple angles
 * - Remove glasses/hat for better recognition
 * 
 * ========================================
 */

/*
 * ========================================
 * WIRING REFERENCE
 * ========================================
 * 
 * PERMANENT CONNECTIONS:
 * - Servo Signal: GPIO 12
 * - Servo VCC: 5V (or external 5V supply)
 * - Servo GND: GND
 * - Trigger Input: GPIO 13 (from main ESP32)
 * - LED: GPIO 33 (built-in)
 * - Power: 5V, GND (5V 2A recommended)
 * 
 * PROGRAMMING (FTDI/USB-TTL):
 * - FTDI TX → ESP32-CAM U0RXD
 * - FTDI RX → ESP32-CAM U0TXD
 * - FTDI GND → ESP32-CAM GND
 * - FTDI 5V → ESP32-CAM 5V
 * - GPIO 0 → GND (ONLY during upload)
 * 
 * ⚠️ CRITICAL:
 * - Connect GPIO 0 to GND BEFORE powering on for upload
 * - Disconnect GPIO 0 from GND after upload
 * - Press RESET button to run program
 * - Use 5V 2A power supply (brown-out prevention)
 * 
 * ========================================
 */

/*
 * ========================================
 * FACE RECOGNITION SETTINGS
 * ========================================
 * 
 * Defined in ESP32 libraries:
 * - FACE_ID_SAVE_NUMBER: 7 (max faces to store)
 * - ENROLL_CONFIRM_TIMES: 5 (enrollments per person)
 * - FACE_RECOGNITION_THRESHOLD: 0.6 (similarity threshold)
 * 
 * To adjust:
 * - Edit values in fr_forward.h
 * - Lower threshold = more lenient recognition
 * - Higher threshold = stricter recognition
 * 
 * ========================================
 */

/*
 * ========================================
 * SYSTEM WORKFLOW
 * ========================================
 * 
 * 1. Main ESP32 IR sensor detects motion
 * 2. Main ESP32 sets GPIO 13 HIGH
 * 3. ESP32-CAM receives trigger
 * 4. ESP32-CAM captures image
 * 5. Neural network detects faces
 * 6. Compare detected face with enrolled faces
 * 7. IF MATCH:
 *    - LED blinks 3 times (success)
 *    - Servo rotates to 90° (unlock)
 *    - Wait 5 seconds
 *    - Servo returns to 0° (lock)
 * 8. IF NO MATCH:
 *    - LED blinks 6 times fast (rejected)
 *    - Door remains locked
 * 9. System ready for next trigger (3s cooldown)
 * 
 * ========================================
 */

/*
 * ========================================
 * TROUBLESHOOTING
 * ========================================
 * 
 * Issue: Brown-out detector triggered
 * - Use 5V 2A power supply (not USB)
 * - Add 470µF capacitor near power pins
 * - Use shorter, thicker power cables
 * 
 * Issue: Camera init failed
 * - Check camera ribbon cable
 * - Ensure OV2640 camera module
 * - Reseat camera connection
 * 
 * Issue: Can't upload code
 * - Connect GPIO 0 to GND before power on
 * - Press RESET while GPIO 0 grounded
 * - Select AI Thinker ESP32-CAM board
 * - Try 115200 upload speed
 * 
 * Issue: Face not detected
 * - Improve lighting (well-lit face)
 * - Correct distance (30-100cm)
 * - Face camera directly
 * - Remove obstructions (glasses, hat)
 * 
 * Issue: Face detected but not recognized
 * - Re-enroll from multiple angles
 * - Enroll in similar lighting as use
 * - Lower recognition threshold
 * - Clear old enrollments
 * 
 * Issue: Servo not moving
 * - Check servo wiring
 * - Use external 5V supply for servo
 * - Test servo with sweep example
 * 
 * ========================================
 */

/*
 * ========================================
 * SECURITY CONSIDERATIONS
 * ========================================
 * 
 * LIMITATIONS:
 * - Can be fooled by high-quality photos
 * - Lighting affects accuracy
 * - Not suitable for high-security applications
 * 
 * IMPROVEMENTS:
 * - Add liveness detection (blink detection)
 * - Implement access logging
 * - Add PIN backup method
 * - Use 3D depth sensing camera
 * - Add tamper detection
 * 
 * BEST PRACTICES:
 * - Hide ESP32-CAM module
 * - Use weatherproof enclosure
 * - Implement access logs
 * - Regular face re-enrollment
 * - Backup unlock method
 * 
 * ========================================
 */
