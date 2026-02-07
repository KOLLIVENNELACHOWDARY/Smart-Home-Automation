# Firebase Setup Guide

Complete step-by-step guide to configure Firebase Realtime Database for the Smart Home Automation System.

---

## 📋 Prerequisites

- Google account
- Active internet connection
- Project credentials ready (WiFi SSID, passwords)

---

## 🔥 Step 1: Create Firebase Project

1. **Go to Firebase Console**
   - Navigate to: [https://console.firebase.google.com](https://console.firebase.google.com)
   - Sign in with your Google account

2. **Create New Project**
   - Click **"Add project"** or **"Create a project"**
   - Enter project name: `smart-home-automation` (or your choice)
   - Click **Continue**

3. **Google Analytics (Optional)**
   - Toggle off if not needed
   - Click **Create project**
   - Wait for project creation (30-60 seconds)
   - Click **Continue** when ready

---

## 🗄️ Step 2: Enable Realtime Database

1. **Navigate to Database**
   - From left sidebar, click **Build** → **Realtime Database**
   - Click **Create Database**

2. **Choose Database Location**
   - Select closest region to your location
   - Example: `us-central1` (United States)
   - Click **Next**

3. **Security Rules**
   - Select **"Start in test mode"** (for development)
   - Click **Enable**
   
   ⚠️ **Warning**: Test mode allows unrestricted access. For production:
   ```json
   {
     "rules": {
       ".read": "auth != null",
       ".write": "auth != null"
     }
   }
   ```

4. **Database Created**
   - You'll see your database URL
   - Format: `https://your-project-id.firebaseio.com/`
   - **Copy this URL** (you'll need it later)

---

## 🔑 Step 3: Get Database Secret Key

1. **Open Project Settings**
   - Click gear icon (⚙️) next to **Project Overview**
   - Select **Project settings**

2. **Navigate to Service Accounts**
   - Click **Service accounts** tab
   - Scroll down to **Database secrets**

3. **Get Legacy Token**
   - Click **Show** next to database secret
   - **Copy the secret key**
   - Keep this secure (don't share publicly)

   Example format: `xX1Xx2xXx3xXxXxXx4xXxXxXxXxXxXxXxXxXxXx`

---

## 📝 Step 4: Configure Database Rules (Important!)

1. **Go to Realtime Database**
   - Left sidebar → **Build** → **Realtime Database**
   - Click **Rules** tab

2. **For Testing (Development)**
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
   - Click **Publish**
   - ⚠️ Warning: Anyone can read/write. Use only for testing!

3. **For Production (Recommended)**
   ```json
   {
     "rules": {
       "home": {
         ".read": true,
         ".write": true,
         "status": {
           ".validate": "newData.isString()"
         },
         "temperature": {
           ".validate": "newData.isNumber()"
         },
         "humidity": {
           ".validate": "newData.isNumber()"
         },
         "smoke": {
           ".validate": "newData.isNumber()"
         }
       }
     }
   }
   ```

4. **Advanced Security (Production with Auth)**
   ```json
   {
     "rules": {
       "home": {
         ".read": "auth != null",
         ".write": "auth != null"
       }
     }
   }
   ```

---

## 💻 Step 5: Update ESP32 Code

1. **Open `esp32_iot_hub.ino`**

2. **Replace Credentials**
   ```cpp
   #define FIREBASE_HOST "your-project-id.firebaseio.com"
   #define FIREBASE_AUTH "your_database_secret_key"
   ```

   **Example:**
   ```cpp
   #define FIREBASE_HOST "smart-home-automation-fe6b7.firebaseio.com"
   #define FIREBASE_AUTH "xX1Xx2xXx3xXxXxXx4xXxXxXxXxXxXxXxXxXxXx"
   ```

   ⚠️ **Important**: 
   - Do NOT include `https://` in FIREBASE_HOST
   - Do NOT include trailing `/` in FIREBASE_HOST

3. **Save and Upload Code**

---

## 🧪 Step 6: Test Connection

1. **Upload Code to ESP32**
   - Compile and upload modified code
   - Open Serial Monitor (9600 baud)

2. **Check Serial Output**
   ```
   [WiFi] Connected!
   [WiFi] IP Address: 192.168.1.100
   [Firebase] Connected!
   [Blynk] Connected!
   ========== SYSTEM READY ==========
   ```

3. **Verify Data in Firebase Console**
   - Go to Firebase Console → Realtime Database → Data tab
   - You should see:
   ```json
   home/
     ├── status: "System Online"
     ├── humidity: 65.5
     ├── temperature: 28.3
     └── smoke: 145
   ```

4. **Real-time Updates**
   - Data should update every 2 seconds
   - You can see live changes in Firebase Console

---

## 📊 Step 7: Understanding Database Structure

### Current Structure
```json
{
  "home": {
    "status": "PUMP_ON|Soil:650",
    "humidity": 65.5,
    "temperature": 28.3,
    "smoke": 145,
    "camera": "TRIGGERED",
    "lastMotion": "1234567890",
    "alerts": "SMOKE_DETECTED"
  }
}
```

### Data Types

| Path | Type | Example | Source |
|------|------|---------|--------|
| `/home/status` | String | "PUMP_ON" | Arduino |
| `/home/humidity` | Float | 65.5 | ESP32 DHT11 |
| `/home/temperature` | Float | 28.3 | ESP32 DHT11 |
| `/home/smoke` | Integer | 145 | ESP32 MQ sensor |
| `/home/camera` | String | "TRIGGERED" | ESP32 IR motion |
| `/home/lastMotion` | String | "1234567890" | ESP32 timestamp |
| `/home/alerts` | String | "SMOKE_DETECTED" | ESP32 alert |

---

## 🔍 Step 8: Reading Data from Firebase

### Via Console
1. Go to **Realtime Database** → **Data** tab
2. Expand `/home` node
3. See all real-time values

### Via REST API
```bash
# Get all data
curl 'https://your-project-id.firebaseio.com/home.json?auth=YOUR_SECRET'

# Get specific value
curl 'https://your-project-id.firebaseio.com/home/temperature.json?auth=YOUR_SECRET'
```

### Via Web Application
```javascript
// JavaScript example
const database = firebase.database();
const ref = database.ref('home/temperature');

ref.on('value', (snapshot) => {
  const temperature = snapshot.val();
  console.log('Temperature:', temperature);
});
```

---

## ✍️ Step 9: Writing Data to Firebase

### Via Console
1. Click on any value in **Data** tab
2. Click **Edit** (pencil icon)
3. Modify value and press **Enter**

### Via ESP32 (Already Implemented)
```cpp
// Set string value
Firebase.setString(fbdo, "/home/status", "LIGHT_ON");

// Set float value
Firebase.setFloat(fbdo, "/home/temperature", 28.5);

// Set integer value
Firebase.setInt(fbdo, "/home/smoke", 150);
```

### Via REST API
```bash
# Set value
curl -X PUT -d '{"temperature": 28.5}' \
  'https://your-project-id.firebaseio.com/home.json?auth=YOUR_SECRET'
```

---

## 🐛 Troubleshooting

### Issue: "Permission Denied" Error

**Solution:**
1. Check database rules (should allow read/write)
2. Verify secret key is correct
3. Ensure database URL is correct

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

### Issue: "Failed to connect to Firebase"

**Solution:**
1. Check internet connection
2. Verify FIREBASE_HOST format (no https://, no trailing /)
3. Confirm WiFi is working
4. Check Firebase project status

### Issue: Data not updating in Firebase

**Solution:**
1. Check Serial Monitor for errors
2. Verify ESP32 WiFi connection
3. Check Firebase.setXXX() return values
4. Add error handling:
   ```cpp
   if (Firebase.setFloat(fbdo, "/home/temperature", temp)) {
     Serial.println("Success");
   } else {
     Serial.println("Error: " + fbdo.errorReason());
   }
   ```

### Issue: "Auth token is invalid"

**Solution:**
1. Regenerate database secret
2. Update FIREBASE_AUTH in code
3. Re-upload code to ESP32

---

## 📱 Step 10: Mobile Access (Optional)

### Web Dashboard

Create simple HTML file to view data:

```html
<!DOCTYPE html>
<html>
<head>
  <title>Smart Home Dashboard</title>
  <script src="https://www.gstatic.com/firebasejs/9.0.0/firebase-app.js"></script>
  <script src="https://www.gstatic.com/firebasejs/9.0.0/firebase-database.js"></script>
</head>
<body>
  <h1>Smart Home Status</h1>
  <div id="status"></div>
  <div id="temperature"></div>
  <div id="humidity"></div>
  
  <script>
    const firebaseConfig = {
      databaseURL: "https://your-project-id.firebaseio.com"
    };
    
    firebase.initializeApp(firebaseConfig);
    const database = firebase.database();
    
    // Listen for temperature changes
    database.ref('home/temperature').on('value', (snapshot) => {
      document.getElementById('temperature').innerHTML = 
        'Temperature: ' + snapshot.val() + '°C';
    });
    
    // Listen for humidity changes
    database.ref('home/humidity').on('value', (snapshot) => {
      document.getElementById('humidity').innerHTML = 
        'Humidity: ' + snapshot.val() + '%';
    });
  </script>
</body>
</html>
```

---

## 🔒 Security Best Practices

### 1. **Protect Database Secret**
- Never commit to public GitHub
- Use environment variables
- Rotate periodically

### 2. **Implement Proper Rules**
```json
{
  "rules": {
    "home": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".validate": "newData.hasChildren(['status', 'temperature', 'humidity'])"
    }
  }
}
```

### 3. **Enable Authentication (Advanced)**
- Use Firebase Authentication
- Implement user accounts
- Restrict access per user

### 4. **Monitor Usage**
- Check Firebase Console → Usage tab
- Set up billing alerts
- Monitor read/write operations

---

## 📊 Firebase Console Features

### Data Tab
- View real-time data
- Edit values manually
- Export data (JSON)
- Import data

### Rules Tab
- Define security rules
- Test rules with simulator
- Publish rule changes

### Usage Tab
- Monitor database reads/writes
- Check storage usage
- View concurrent connections

### Backups (Paid Plans)
- Automated daily backups
- Point-in-time recovery
- Export to Google Cloud Storage

---

## 🎯 Next Steps

1. ✅ Firebase configured and working
2. ⬜ Set up Blynk (see BLYNK_SETUP.md)
3. ⬜ Create custom dashboard
4. ⬜ Implement authentication
5. ⬜ Add data analytics

---

## 📚 Additional Resources

- [Firebase Documentation](https://firebase.google.com/docs)
- [Realtime Database Guide](https://firebase.google.com/docs/database)
- [Security Rules Reference](https://firebase.google.com/docs/database/security)
- [REST API Reference](https://firebase.google.com/docs/reference/rest/database)

---

**Setup Complete! Your Firebase Realtime Database is ready for Smart Home Automation.**
