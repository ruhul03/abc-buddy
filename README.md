# 🤖 ABC Buddy — Complete 6-Feature Platform

ABC Buddy is an interactive smart educational companion robot powered by ESP32, ESP32-CAM, I2S Audio, and a Google GenAI Backend (**Gemini 3.6 Flash** / **Gemini 2.5 Flash Lite** + **Edge-TTS** + **miniaudio**).

---

## 🌟 The 6 Core Features

### 5.1 Feature 1: Child Following and Obstacle Avoidance
- **4WD Mobility**: Powered by an L298N motor driver using hardware LEDC PWM on ESP32.
- **Child Following**: Measures distance to the child in real time; if the child is within 40–75cm, Buddy smoothly follows forward. If too far (>110cm) or close enough (<40cm), it holds position safely.
- **Obstacle Avoidance**: HC-SR04 ultrasonic sensor scans ahead. Obstacles under 30cm trigger an emergency stop, reverse backup, and steering maneuver.
- **Cliff Sensor Architecture**: Pre-configured modular hook (`#define ENABLE_CLIFF_SENSORS false`) ready for underbody IR sensors.

### 5.2 Feature 2: Voice-to-Voice Q&A
- Child presses **"🎙️ Press to Talk with Buddy"** on the dashboard.
- I2S INMP441 microphone captures the question in crystal-clear audio.
- Audio streams to `/ask`, ingested natively into **Gemini 3.6 Flash** (no external STT needed).
- **Age-Adaptive Responses**: Prompts adapt to child age (Ages 3–5: simple & playful; Ages 6–8: elementary tutor; Ages 9+: scientific explanation).
- Answers are synthesized via Edge-TTS and played aloud through the robot's PAM8403 amplifier. All conversations are logged in SQLite for parent review.

### 5.3 Feature 3: Environmental Alerts
- **Real-Time Monitoring**: MQ-2 sensor monitors smoke and gas; DS18B20 probe monitors ambient temperature.
- **Local Multi-Part Alarm**: Crossing safety thresholds triggers the OLED **Hazard Face** (`! HAZARD ALERT !`), stops motors instantly, and plays an audible spoken emergency warning through the robot's speaker.
- **Cloud Notification**: Telemetry is automatically pushed to the backend (`/telemetry_ingest`), alerting the parent portal.

### 5.4 Feature 4: Child Cry Detection with Photo Alert
- The system monitors audio for acoustic distress/crying patterns.
- High-confidence events validated via Gemini audio classification automatically trigger the **ESP32-CAM** to snap a photo of the room.
- The photo is saved and posted with a timestamp to the **Parent Cloud Portal** (`/parent`), and Buddy speaks a comforting reassurance (*"Don't worry, little friend! Buddy is right here with you."*).
- Parents can test this anytime using the **🚨 Test Child Cry Alert** button.

### 5.5 Feature 5: Handwriting and Letter Recognition
- **OLED Target Display**: When a practice session starts, the robot's OLED screen shows **"PRACTICE WRITING"** with a large, bold target letter (e.g. `B`) so the child knows what to draw.
- **Camera Vision Evaluation**: Child holds the drawing in front of the ESP32-CAM.
- **Gemini 3.6 Flash Vision**: Analyzes the drawing, provides warm praise, and offers 1 concrete, encouraging tip for stroke improvement.
- The spoken feedback plays through the robot's speaker, and the attempt is logged in the **Handwriting Learning Journal**.

### 5.6 Feature 6: Remote Monitoring & Parent Portal
- **Local Control Page (`http://<ESP32_IP>/`)**: Low-latency manual D-pad driving, speed slider, live distance/gas/temp gauges, and practice controls.
- **Parent Cloud Portal (`http://<SERVER_IP>:5000/parent`)**:
  - Live robot telemetry and environmental trend history.
  - Child distress and cry alert gallery with captured room photos.
  - Handwriting practice journal with snapshots and AI reviews.
  - Voice conversation transcripts.
  - Remote parent announcement broadcaster (*"Speak Announcement to Robot"*).
  - Child age configuration dropdown.

---

## 📁 Project Structure

```
ABC_Buddy/
├── ABC_Buddy.ino   # ESP32 Arduino C++ firmware (Web server, /play_audio streaming, sensors, motors, OLED)
├── server.py       # Python Backend (Gemini 3.6 Flash, Edge-TTS, SQLite DB, Parent Portal)
├── buddy_data.db   # SQLite database storing logs, telemetry, and practice sessions
├── static/alerts/  # Stored photos from cry detection and handwriting practice
└── README.md       # Complete 6-feature documentation
```

---

## 🚀 Getting Started

### 1. Run the Python AI Backend & Parent Portal
On your computer (on the same Wi-Fi network as the ESP32):

```bash
# Start backend server
python server.py
```
- **Backend API**: `http://localhost:5000/`
- **Parent Cloud Portal**: `http://localhost:5000/parent`

---

### 2. Upload `ABC_Buddy.ino` to Main ESP32
1. **Important**: If you already had Arduino IDE open, close and re-open Arduino IDE (or click File -> Open -> `ABC_Buddy.ino`) so Arduino IDE reloads the clean sketch along with the `index_html.h` tab!
2. Upload to your main ESP32 board (`ESP32 Dev Module`).

---

### 3. Hardware UART Link: Main Kit <-> ESP32-CAM
Connect the hardware Serial lines between the Main Kit and ESP32-CAM:
- **Main Kit rox** (Default GPIO 16 / RX2) ➔ **ESP32-CAM IO12** (TX)
- **Main Kit tox** (Default GPIO 17 / TX2) ➔ **ESP32-CAM IO13** (RX)
- **Main Kit GND** ➔ **ESP32-CAM GND** (Crucial: shared common ground)

*(Optional: Upload [`ESP32_CAM_Companion.ino`](file:///c:/Users/asmru/Downloads/ABC_Buddy/ESP32_CAM_Companion.ino) to your ESP32-CAM for automatic IP discovery over Serial and hardware SNAP commands!)*

---

### 4. Open the Dashboards
- **Robot Control Page**: Open `http://<ESP32_IP>/` in your browser.
- **Parent Portal**: Click the **👨‍👩‍👧 Parent Portal** button in the header or go directly to `http://<YOUR_PC_IP>:5000/parent`.

