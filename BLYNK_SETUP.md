# Blynk Setup Guide

Complete step-by-step guide to configure Blynk IoT platform for mobile monitoring and control of your Smart Home Automation System.

---

## 📋 Prerequisites

- Smartphone (iOS or Android)
- Active internet connection
- ESP32 code ready for modification
- Firebase already configured (optional, but recommended)

---

## 📱 Step 1: Install Blynk App

### For iOS
1. Open **App Store**
2. Search for **"Blynk IoT"**
3. Download and install **Blynk IoT** by Blynk Inc.

### For Android
1. Open **Google Play Store**
2. Search for **"Blynk IoT"**
3. Download and install **Blynk IoT** by Blynk Inc.

### Alternative: Legacy Blynk (Optional)
- For older projects, you can also use **"Blynk - IoT for Arduino, ESP32"**
- This guide uses the newer **Blynk IoT** platform

---

## 👤 Step 2: Create Blynk Account

1. **Open Blynk App**
   - Launch the installed app

2. **Sign Up**
   - Tap **"Create New Account"**
   - Enter your email address
   - Create a password (min 8 characters)
   - Tap **"Sign Up"**

3. **Verify Email**
   - Check your email inbox
   - Click verification link
   - Return to app

4. **Sign In**
   - Enter your credentials
   - Tap **"Login"**

---

## 🏠 Step 3: Create New Project (Template)

### Using Blynk IoT (New Version)

1. **Create Template**
   - Open Blynk.Console (web): [https://blynk.cloud](https://blynk.cloud)
   - Sign in with your account
   - Click **"New Template"**

2. **Template Settings**
   - Name: `Smart Home Automation`
   - Hardware: `ESP32`
   - Connection Type: `WiFi`
   - Click **"Done"**

3. **Configure Datastreams**
   - Go to **Datastreams** tab
   - Click **"New Datastream"** → **"Virtual Pin"**
   - Create following datastreams:

#### Datastream 1: Temperature
```
Name: Temperature
Pin: V1
Data Type: Double
Units: °C
Min: -20
Max: 50
Decimals: 1
Default: 0
```

#### Datastream 2: Arduino Status
```
Name: Arduino Status
Pin: V5
Data Type: String
Default: "Offline"
```

#### Datastream 3: Humidity
```
Name: Humidity
Pin: V6
Data Type: Double
Units: %
Min: 0
Max: 100
Decimals: 1
Default: 0
```

#### Datastream 4: Smoke Level
```
Name: Smoke Level
Pin: V7
Data Type: Integer
Min: 0
Max: 4095
Default: 0
```

4. **Save Template**
   - Click **"Save"** at top right

---

## 📊 Step 4: Design Mobile Dashboard

1. **Go to Web Dashboard**
   - Click **"Web Dashboard"** tab
   - Or use **Mobile Dashboard** for phone layout

2. **Add Widgets**

### Widget 1: Temperature Display
- Drag **"Gauge"** or **"Value Display"** widget
- Settings:
  - Title: `Temperature`
  - Datastream: `V1 - Temperature`
  - Units: `°C`
  - Color: Blue
  - Range: -20 to 50

### Widget 2: Humidity Display
- Drag **"Gauge"** or **"Value Display"** widget
- Settings:
  - Title: `Humidity`
  - Datastream: `V6 - Humidity`
  - Units: `%`
  - Color: Light blue
  - Range: 0 to 100

### Widget 3: Smoke Level Gauge
- Drag **"Gauge"** widget
- Settings:
  - Title: `Smoke Level`
  - Datastream: `V7 - Smoke Level`
  - Color: Red/Orange
  - Range: 0 to 4095
  - Danger zone: 400+

### Widget 4: Arduino Status
- Drag **"Label"** or **"Terminal"** widget
- Settings:
  - Title: `System Status`
  - Datastream: `V5 - Arduino Status`
  - Font: Medium
  - Color: Green

### Widget 5: Last Update (Optional)
- Drag **"Label"** widget
- Settings:
  - Title: `Last Updated`
  - Mode: Show timestamp

3. **Arrange Layout**
   - Drag widgets to desired positions
   - Resize for better view
   - Click **"Save"** when done

---

## 🔔 Step 5: Configure Events (Notifications)

1. **Go to Events Tab**
   - In template settings
   - Click **"Events"** → **"New Event"**

### Event 1: Intruder Alert
```
Name: intruder
Description: Person detected at door
Event Code: intruder
Notification:
  - Title: "🚨 Intruder Alert"
  - Body: "Person detected at main door!"
  - Priority: High
  - Sound: Enabled
```

### Event 2: Smoke Alert
```
Name: smoke_alert
Description: High smoke level detected
Event Code: smoke_alert
Notification:
  - Title: "🔥 Smoke Alert"
  - Body: "High smoke detected in home!"
  - Priority: Critical
  - Sound: Enabled
```

2. **Save Events**

---

## 🔑 Step 6: Get Auth Token

### Method 1: Via Web Console
1. Click **"Devices"** in left sidebar
2. Click **"New Device"**
3. Select your template: `Smart Home Automation`
4. Name: `ESP32 Smart Home`
5. Click **"Create"**
6. **Copy the Auth Token** displayed
7. Keep this token secure

### Method 2: Via Email (Legacy)
1. In mobile app, open your project
2. Tap settings icon (⚙️)
3. Tap **"Email Auth Token"**
4. Check your email
5. Copy the token from email

**Example Token Format:**
```
a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6
```

---

## 💻 Step 7: Update ESP32 Code

1. **Open `esp32_iot_hub.ino`**

2. **Add Blynk Auth Token**
   ```cpp
   #define BLYNK_AUTH "a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6"
   ```

3. **Verify Virtual Pin Mapping**
   ```cpp
   // Already implemented in code:
   Blynk.virtualWrite(V1, temperature);  // Temperature
   Blynk.virtualWrite(V5, msg);           // Arduino Status
   Blynk.virtualWrite(V6, humidity);      // Humidity
   Blynk.virtualWrite(V7, smokeLevel);    // Smoke Level
   ```

4. **Verify Events**
   ```cpp
   // Already implemented:
   Blynk.logEvent("intruder", "Person detected at door");
   Blynk.logEvent("smoke_alert", "High smoke detected!");
   ```

5. **Save and Upload Code**

---

## 🧪 Step 8: Test Connection

1. **Upload Code to ESP32**
   - Compile and upload
   - Open Serial Monitor (9600 baud)

2. **Check Serial Output**
   ```
   [WiFi] Connected!
   [Blynk] Connected!
   ========== SYSTEM READY ==========
   ```

3. **Open Blynk App**
   - You should see device **"Online"**
   - Widgets should show real data
   - Temperature, humidity updating every 2 seconds

4. **Test Notifications**
   - Wave hand over IR sensor
   - Should receive "Intruder Alert" notification
   - Blow smoke near MQ sensor
   - Should receive "Smoke Alert" notification

---

## 📊 Understanding Virtual Pins

### Virtual Pin Map

| Virtual Pin | Purpose | Data Type | Update Frequency |
|-------------|---------|-----------|------------------|
| V1 | Temperature (°C) | Float | Every 2 seconds |
| V5 | Arduino Status | String | Real-time |
| V6 | Humidity (%) | Float | Every 2 seconds |
| V7 | Smoke Level (0-4095) | Integer | Every 2 seconds |

### How Virtual Pins Work

```cpp
// WRITE to Blynk (ESP32 → App)
Blynk.virtualWrite(V1, temperature);  // Send temperature to app

// READ from Blynk (App → ESP32)
BLYNK_WRITE(V8) {  // Example for button on V8
  int value = param.asInt();  // Get button state
  // Control something based on app input
}
```

---

## 🎨 Step 9: Customize Dashboard

### Color Coding
- **Green**: Normal operation
- **Yellow**: Warning (smoke 200-400)
- **Red**: Critical (smoke >400)
- **Blue**: Information

### Add Charts (Optional)
1. Drag **"SuperChart"** widget
2. Add datastreams:
   - V1 (Temperature) - Line chart
   - V6 (Humidity) - Line chart
3. Time range: 1 hour, 6 hours, or 24 hours
4. See historical trends

### Add Buttons for Control (Future Enhancement)
1. Drag **"Button"** widget
2. Assign to new virtual pin (e.g., V10)
3. Implement control in ESP32 code:
   ```cpp
   BLYNK_WRITE(V10) {
     int value = param.asInt();
     if (value == 1) {
       // Turn something ON
     } else {
       // Turn something OFF
     }
   }
   ```

---

## 📱 Step 10: Mobile App Features

### Device Control
- View real-time sensor data
- Receive push notifications
- See device online/offline status
- View historical data (charts)

### Widgets Available
- **Value Display**: Show single value
- **Gauge**: Show value with range indicator
- **SuperChart**: Historical data graphs
- **Terminal**: Scrolling text output
- **Button**: Control on/off
- **Slider**: Control variable values
- **LED**: Status indicator

### Automation (Advanced)
- Set up automations in Blynk.Console
- Example: If smoke > 400, send email
- Example: If temperature > 35°C, alert

---

## 🐛 Troubleshooting

### Issue: "Device Offline" in Blynk App

**Solution:**
1. Check ESP32 WiFi connection
2. Verify Auth Token is correct
3. Ensure ESP32 has internet access
4. Check Serial Monitor for Blynk errors
5. Restart ESP32

### Issue: Widgets Not Updating

**Solution:**
1. Verify virtual pin assignments match
2. Check datastream configuration
3. Ensure `Blynk.run()` is in loop()
4. Add debug output:
   ```cpp
   Serial.println("Writing to Blynk V1: " + String(temp));
   Blynk.virtualWrite(V1, temp);
   ```

### Issue: Notifications Not Working

**Solution:**
1. Enable notifications in phone settings
2. Check event codes match exactly
3. Verify events are enabled in template
4. Test with:
   ```cpp
   Blynk.logEvent("intruder", "Test notification");
   ```

### Issue: "Auth Token is invalid"

**Solution:**
1. Regenerate Auth Token in Blynk.Console
2. Update token in code
3. Re-upload code to ESP32

### Issue: Connection Timeout

**Solution:**
1. Check internet stability
2. Try different Blynk server region
3. Increase timeout:
   ```cpp
   Blynk.config(BLYNK_AUTH, "blynk.cloud", 80);
   ```

---

## 🔒 Security & Limits

### Free Plan Limits
- **Devices**: Up to 2 devices
- **Datastreams**: Up to 10 per device
- **Events**: Up to 5 per template
- **Notifications**: 100 per day
- **API calls**: Limited

### Paid Plans
- **PRO**: More devices, unlimited datastreams
- **Business**: Team collaboration, white label
- Check [blynk.io/pricing](https://blynk.io/pricing)

### Security Best Practices
1. **Never share Auth Token publicly**
2. **Don't commit token to GitHub**
3. **Use different tokens for each device**
4. **Rotate tokens periodically**
5. **Monitor device activity in console**

---

## 📊 Advanced Features

### Webhooks
Send data to external services:
```cpp
// Send HTTP request to custom server
Blynk.virtualWrite(V20, "http://yourserver.com/api");
```

### Email Notifications
```cpp
Blynk.email("your@email.com", "Subject", "Message body");
```

### SMS Notifications (Paid Feature)
```cpp
Blynk.sms("+1234567890", "Alert message");
```

### Data Export
- Export historical data to CSV
- Available in Blynk.Console → Device → Export

---

## 🎯 Widget Examples

### Temperature with Color Ranges
```
Green:  < 25°C  (Normal)
Yellow: 25-30°C (Warm)
Orange: 30-35°C (Hot)
Red:    > 35°C  (Critical)
```

### Smoke Level Gauge
```
Normal:  0-200   (Green)
Warning: 200-400 (Yellow)
Danger:  400+    (Red)
```

### Status Terminal
Displays Arduino messages:
```
PUMP_ON|Soil:650
LIGHT_ON|Motion
GATE_OPEN|Dist:15cm
COVER_CLOSE|Rain
```

---

## 🔄 Data Flow

```
ESP32 Sensors → ESP32 Processing
                     ↓
              Blynk.virtualWrite()
                     ↓
              Blynk Cloud Server
                     ↓
              Mobile App Widgets
                     ↓
              User sees real-time data
```

---

## 📚 Code Examples

### Read Button from Blynk App
```cpp
BLYNK_WRITE(V10) {
  int buttonState = param.asInt();
  Serial.print("Button V10: ");
  Serial.println(buttonState);
  
  if (buttonState == 1) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
```

### Send Alert Based on Condition
```cpp
if (temperature > 35) {
  Blynk.logEvent("temp_alert", "Temperature critical!");
  Blynk.virtualWrite(V1, temperature);
}
```

### Update Multiple Widgets
```cpp
Blynk.virtualWrite(V1, temperature);
Blynk.virtualWrite(V6, humidity);
Blynk.virtualWrite(V7, smokeLevel);
Blynk.virtualWrite(V5, "All systems normal");
```

---

## 🌐 Blynk Web Dashboard

Access via browser: [https://blynk.cloud](https://blynk.cloud)

### Features
- Same dashboard as mobile app
- Larger screen for better visualization
- Easier widget configuration
- Device management
- User management
- Automation setup
- Analytics and reports

---

## 🎓 Learning Resources

- **Blynk Documentation**: [docs.blynk.io](https://docs.blynk.io)
- **Blynk Community**: [community.blynk.cc](https://community.blynk.cc)
- **Example Projects**: [examples.blynk.cc](https://examples.blynk.cc)
- **YouTube Tutorials**: Search "Blynk ESP32 tutorial"

---

## 🚀 Next Steps

1. ✅ Blynk configured and working
2. ⬜ Create custom automations
3. ⬜ Add control widgets (buttons, sliders)
4. ⬜ Set up data analytics
5. ⬜ Explore advanced features (webhooks, email)

---

## 📞 Support

- **Blynk Support**: support@blynk.io
- **Community Forum**: [community.blynk.cc](https://community.blynk.cc)
- **Documentation**: [docs.blynk.io](https://docs.blynk.io)

---

**Setup Complete! Your Blynk mobile dashboard is ready for Smart Home monitoring.**
