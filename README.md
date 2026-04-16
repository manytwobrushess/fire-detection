
# 🔥 IoT Fire Detection & Monitoring System

An integrated IoT solution that monitors environmental safety (Fire, Gas, Temperature) using an Arduino UNO R4 WiFi. It features a real-time web dashboard and instant alerts via a Telegram Bot.

## 🚀 Features
* **Real-time Monitoring:** Tracks Temperature and Gas levels continuously.
* **Instant Alerts:** Triggers visual/audio alarms and Telegram notifications immediately upon flame detection.
* **Live Dashboard:** Visualizes historical data using Chart.js and maintains a searchable log table.
* **Non-Blocking Logic:** Uses efficient timer-based multitasking on the Arduino.

## 📋 Prerequisites
Before running the project, ensure you have the following installed:
* **Arduino IDE** (or VS Code with PlatformIO extension)
* **Python 3.x** (for the Telegram Bot)
* **Git** (to clone the repository)
* A modern web browser (Chrome, Firefox, or Edge)

## 🔌 Pin Configuration
| Component | Pin (Arduino UNO R4) |
| :--- | :--- |
| **Flame Sensor** | D2 |
| **Buzzer** | D8 |
| **Red LED** | D9 |
| **Green LED** | D10 |
| **Blue LED** | D11 |
| **MQ2 Gas Sensor** | A0 |
| **NTC Thermistor** | A1 |
| **LCD Display** | I2C (SDA/SCL) |

### 1. Get the Code
Open your terminal or command prompt and run:
```bash
git clone [https://github.com/YOUR_USERNAME/Fire-detection.git](https://github.com/YOUR_USERNAME/Fire-detection.git)
cd Fire-detection 
```

### 2. Backend (Arduino)
1.  Navigate to the `backend` folder.
2.  Open `backend.ino` in the Arduino IDE (or VS Code with PlatformIO).
3.  Install the required libraries via Library Manager:
    * `WiFiS3` (for UNO R4 WiFi)
    * `NTPClient` (by Fabrice Weinberg)
    * `LiquidCrystal_I2C` (by Frank de Brabander)
4.  Update `network_config.h` with your local WiFi SSID and Password.
5.  **Note:** The code contains pre-configured Firebase keys (Read-Only). You do not need to change them unless you want to enable writing data.
6.  Upload the code to your Arduino UNO R4 WiFi.

### 3. Frontend (Dashboard)
1.  Navigate to the `frontend` folder.
2.  Open `index.html` in any modern web browser (Chrome, Edge, Firefox).
3.  The dashboard will automatically connect to the Firebase database and load historical data.
    * *No local server installation is required.*

### 4. Telegram Bot
1.  Navigate to the `telegrambot` folder.
2.  Ensure you have Python installed.
3.  Install the required library:
    ```bash
    pip install python-telegram-bot
    ```
4.  Run the bot script:
    ```bash
    python bot.py
    ```

## Project Structure
This repository is organized into three main components:
| Folder | Description |
| :--- | :--- |
| **/backend** | Arduino C++ code running on the UNO R4 WiFi. Handles sensors (Flame, MQ2, NTC) and data upload. |
| **/frontend** | A web-based dashboard (HTML/JS/Bootstrap) to visualize real-time data charts and logs. |
| **/telegrambot** | The bot script that receives system flags and sends alerts to user phones. |

## ⚠️ Known Limitations

While this system functions effectively as a prototype, the following limitations should be noted:

### 1. Database Write Restrictions (Read-Only Mode)
* **Issue:** The Firebase database is currently configured in **Read-Only Mode** for public security.
* **Impact:** The Arduino backend successfully connects to WiFi but **cannot upload new sensor logs**. The Dashboard visualizes *historical* data perfectly but will not update with *live* data from the device unless the user provides their own private API keys.

### 2. Network Dependency
* **Issue:** The system relies on an active WiFi connection for time synchronization (NTP) and cloud logging.
* **Impact:** If the WiFi connection is lost, the `timeString` generation may default to placeholders ("00:00:00"), and data logging will pause. However, **local safety features** (Buzzer/LEDs) continue to function offline due to the non-blocking architecture.

### 3. Hardcoded Credentials
* **Issue:** Firebase API keys and WiFi credentials are currently hardcoded in `network_config.h` and `config.js`.
* **Impact:** This is acceptable for a prototype but poses a security risk for production. A production version would use environment variables or a captive portal for secure configuration.

### 4. Sensor Calibration
* **Issue:** The Flame and MQ2 Gas sensors use analog potentiometers for sensitivity adjustments.
* **Impact:** False positives (triggering without fire) or false negatives (missing a fire) may occur if the hardware is not manually calibrated to the specific ambient environment.

## 👥 Contributors

* **Law Yee Qi, Sin Zi Yu, Harshini A/P Manimaaran** - *Hardware Implementation & Base Firmware*
    * Designed the circuit and wrote the initial sensor logic (`sensors.cpp`).
* **Ong Yee Xuan** - *IoT Integration, Cloud Backend & Frontend Dashboard*
    * Merged hardware logic with WiFi/Firebase connectivity.
    * Developed the web-based dashboard and configured the real-time database.
* **Muhammad Aiman Bin Rosli @ Saufi** - *Telegram Bot Development*
    * Created the Python script for mobile alerts and remote monitoring.


## 📝 License
This project is open-source.
