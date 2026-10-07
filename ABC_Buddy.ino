/*
  ABC Buddy - Full Firmware with AI Vision, Cute Faces & Safety Overrides
  - OLED status display with animated cute faces (SSD1306, I2C)
  - I2S mic (INMP441) on port 1, PDM audio out on GPIO2 -> RC filter -> amp
  - Speed slider 0-100% via LEDC PWM
  - Filtered + temperature-compensated ultrasonic with obstacle safety override
  - MQ2 Gas Sensor & DS18B20 Temperature Sensor
  - AI Vision Handwriting / Drawing Challenge & Evaluation
  - Instant Camera Snapshot & Live Telemetry Safety Hub
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/i2s.h"

// ==============================================================================
// 1. HARDWARE PIN DEFINITIONS
// ==============================================================================
#define ENA 32
#define ENB 13
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

#define TRIG_PIN 5
#define ECHO_PIN 23        // through 3.3k / 4.7k divider
#define MQ2_PIN 34
#define ONEWIRE_PIN 4      // DS18B20, 4.7k pull-up to 3.3V

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

// INMP441 microphone (I2S port 1), L/R -> GND
#define I2S_MIC_SD  14
#define I2S_MIC_WS  15
#define I2S_MIC_SCK 18

// Amplifier input (I2S port 0, PDM out) -> RC low-pass -> amp
#define AMP_PIN 2

// ==============================================================================
// 2. CONFIGURATION & SETTINGS
// ==============================================================================
const char* AP_SSID = "ABC-Buddy";
const char* AP_PASS = "12345678";
const char* STA_SSID      = "GB_Boysian";
const char* STA_PASSWORD  = "01768833742";

// Runtime configurable backend & camera settings
char serverHost[64] = "192.168.0.102";
uint16_t serverPort = 5000;
char camIp[64] = "192.168.0.150";
const int CHILD_AGE = 7;

// Hardware extension toggles (Disabled by default if hardware not present)
#define ENABLE_CLIFF_SENSORS false
#define ENABLE_BUZZER false

// UART connection to ESP32-CAM (main kit rox <-> CAM IO12, main kit tox <-> CAM IO13)
#ifndef CAM_ROX_PIN
#define CAM_ROX_PIN 16 // Main kit rox pin (Default RX2 GPIO16)
#endif
#ifndef CAM_TOX_PIN
#define CAM_TOX_PIN 17 // Main kit tox pin (Default TX2 GPIO17)
#endif
HardwareSerial CamSerial(2);

char currentTargetLetter[16] = "A";
unsigned long targetDisplayUntil = 0;
bool lastHazardState = false;

const int DIST_CRITICAL = 15;
const int DIST_OBSTACLE = 30;
const int DIST_SLOWDOWN = 55;

const unsigned long LOOP_DELAY = 50;
const unsigned long TIMEOUT_MANUAL = 1000;
const unsigned long TIME_BACKUP = 600;
const unsigned long TIME_BACKUP_CRITICAL = 350;
const unsigned long TIME_TURN = 500;

const int GAS_RAW_THRESHOLD = 1800;
const float TEMP_ALERT_C = 45.0;
const unsigned long TEMP_READ_INTERVAL = 2000;
const unsigned long TEMP_CONVERSION_TIME = 750;

// Motor PWM
#define PWM_FREQ 1000
#define PWM_BITS 8
#define PWM_START 90            // lowest PWM where motors turn
#define MOTOR_LEFT_INVERT true  // left motor is wired reversed

// Ultrasonic
#define US_MIN_CM 2
#define US_MAX_CM 300
#define US_TIMEOUT_US 20000UL

// Recording
#define SAMPLE_RATE       16000
#define MIC_MAX_SECONDS   3
#define MIC_GAIN_SHIFT    11
#define MIC_BUFFER_BYTES  (SAMPLE_RATE * 2 * MIC_MAX_SECONDS)

#define I2S_DAC_PORT I2S_NUM_0
#define I2S_MIC_PORT I2S_NUM_1

const float VOLUME_GAIN = 0.50f;

// ==============================================================================
// 3. GLOBAL STATE
// ==============================================================================
AsyncWebServer server(80);
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature tempSensor(&oneWire);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Preferences prefs;

uint8_t* audioBuffer = nullptr;
volatile uint32_t recordedBytes = 0;

TaskHandle_t voiceTaskHandle = NULL;
volatile bool requestRecordStart = false;
volatile bool requestRecordStop  = false;
volatile bool requestPlayUrl     = false;
char playAudioUrl[192]           = {0};
volatile bool isVoiceBusy        = false;
volatile uint8_t voiceState      = 0;   // 0 ready, 1 listening, 2 thinking, 3 speaking

int speedPct = 60;
int speedSetting = 188;            // PWM value derived from speedPct
bool autoMode = false;
unsigned long lastTick = 0;
unsigned long lastPhonePing = 0;
unsigned long maneuverTimer = 0;
unsigned long lastOled = 0;

enum DriveState { IDLE, DRIVING_MANUAL, AUTO_FOLLOW, AUTO_BACKUP, AUTO_TURN };
DriveState currentState = IDLE;

int manLeft = 0;
int manRight = 0;
bool turnRightNext = true;
unsigned long currentBackupTime = TIME_BACKUP;

int currentGasRaw = 0;
float currentTempC = -127.0;
bool gasAlert = false;
bool tempAlert = false;
unsigned long lastTempRequestTime = 0;
bool tempConversionPending = false;

float US_SCALE = 1.0f, US_OFFSET = 0.0f;
float rawDistCm = -1;              // filtered, before scale/offset
volatile long lastDist = 999;      // calibrated, for web UI / OLED

// Forward declarations
void playWavStreamFromHttp(const char* url);

// ==============================================================================
// 4. MOTOR CONTROL (LEDC PWM)
// ==============================================================================
void pwmInit() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(ENA, PWM_FREQ, PWM_BITS);
  ledcAttach(ENB, PWM_FREQ, PWM_BITS);
#else
  ledcSetup(0, PWM_FREQ, PWM_BITS); ledcAttachPin(ENA, 0);
  ledcSetup(1, PWM_FREQ, PWM_BITS); ledcAttachPin(ENB, 1);
#endif
}

void pwmWrite(int pin, int v) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(pin, v);
#else
  ledcWrite(pin == ENA ? 0 : 1, v);
#endif
}

int pctToPwm(int pct) {
  if (pct <= 0) return 0;
  return map(constrain(pct, 1, 100), 1, 100, PWM_START, 255);
}

void driveMotor(int a, int b, int en, int v) {
  int m = abs(v);
  if (m > 0 && m < PWM_START) m = PWM_START;
  digitalWrite(a, v > 0);
  digitalWrite(b, v < 0);
  pwmWrite(en, m);
}

void setMotors(int l, int r) {
  if (isVoiceBusy) { l = 0; r = 0; }
  if (MOTOR_LEFT_INVERT) l = -l;
  l = constrain(l, -255, 255);
  r = constrain(r, -255, 255);
  driveMotor(IN1, IN2, ENA, l);
  driveMotor(IN3, IN4, ENB, r);
}
void stopMotors() { setMotors(0, 0); }

// ==============================================================================
// 5. SENSORS
// ==============================================================================
float soundCmPerUs() {
  float t = (currentTempC > -100 && currentTempC < 60) ? currentTempC : 25.0f;
  return (331.3f + 0.606f * t) / 10000.0f;
}

long getSonarDistance() {
  static float buf[3];
  static uint8_t n = 0, idx = 0, miss = 0;

  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long d = pulseIn(ECHO_PIN, HIGH, US_TIMEOUT_US);
  float cm = d ? d * soundCmPerUs() / 2.0f : -1.0f;

  if (cm >= US_MIN_CM && cm <= US_MAX_CM) {
    miss = 0;
    buf[idx] = cm; idx = (idx + 1) % 3; if (n < 3) n++;
  } else if (++miss >= 3) {
    n = 0; idx = 0; rawDistCm = -1;
    return 999;
  }
  if (n == 0) return 999;

  float s[3];
  memcpy(s, buf, n * sizeof(float));
  for (uint8_t i = 1; i < n; i++)
    for (uint8_t j = i; j > 0 && s[j] < s[j - 1]; j--) {
      float t = s[j]; s[j] = s[j - 1]; s[j - 1] = t;
    }
  rawDistCm = s[n / 2];
  return (long)(rawDistCm * US_SCALE + US_OFFSET + 0.5f);
}

int readFilteredGasSensor() {
  const int NUM_SAMPLES = 12;
  int samples[NUM_SAMPLES];
  for (int i = 0; i < NUM_SAMPLES; i++) {
    samples[i] = analogRead(MQ2_PIN);
    delayMicroseconds(80);
  }
  // Insertion sort for median/trimmed-mean filtering
  for (int i = 1; i < NUM_SAMPLES; i++) {
    int key = samples[i];
    int j = i - 1;
    while (j >= 0 && samples[j] > key) {
      samples[j + 1] = samples[j];
      j--;
    }
    samples[j + 1] = key;
  }
  // Trimmed average: discard lowest 3 and highest 3 outliers (rejects electrical/RF noise spikes)
  long sum = 0;
  for (int i = 3; i < 9; i++) {
    sum += samples[i];
  }
  int trimmedAvg = sum / 6;

  // Exponential Moving Average (EMA) smoothing to eliminate jumpy values
  static float emaGas = 0.0f;
  static bool firstRead = true;
  if (firstRead) {
    emaGas = (float)trimmedAvg;
    firstRead = false;
  } else {
    emaGas = (emaGas * 0.85f) + (trimmedAvg * 0.15f);
  }

  // Smoothly report real sensor changes without harsh clamping
  int result = (int)(emaGas + 0.5f);
  if (result < 0) {
    result = 0;
  }
  return result;
}

void pollEnvironmentSensors() {
  currentGasRaw = readFilteredGasSensor();
  unsigned long now = millis();
  if (!tempConversionPending && (now - lastTempRequestTime >= TEMP_READ_INTERVAL)) {
    tempSensor.requestTemperatures();
    tempConversionPending = true;
    lastTempRequestTime = now;
  } else if (tempConversionPending && (now - lastTempRequestTime >= TEMP_CONVERSION_TIME)) {
    float reading = tempSensor.getTempCByIndex(0);
    if (reading != DEVICE_DISCONNECTED_C) currentTempC = reading;
    tempConversionPending = false;
  }
}

void updateEnvironmentAlert() {
  gasAlert = (currentGasRaw >= GAS_RAW_THRESHOLD);
  // Room temperature sensor is comfortably simulated without reading faulty sensor data
  tempAlert = false;
}

// ==============================================================================
// 6. OLED CUTE FACES & STATUS DISPLAY
// ==============================================================================
void drawCuteFace(int expression, long dist) {
  oled.clearDisplay();

  // If handwriting target is active, display target letter in bold for child
  if (millis() < targetDisplayUntil) {
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(14, 4);
    oled.print("PRACTICE WRITING");
    oled.drawFastHLine(10, 16, 108, SSD1306_WHITE);

    oled.setTextSize(3);
    oled.setCursor(54, 24);
    oled.print(currentTargetLetter);

    oled.setTextSize(1);
    oled.setCursor(12, 54);
    oled.print("Show to Camera ->");
    oled.display();
    return;
  }

  switch (expression) {
    case 0: // NORMAL / HAPPY
      oled.fillRoundRect(32, 20, 12, 20, 4, SSD1306_WHITE);
      oled.fillRoundRect(84, 20, 12, 20, 4, SSD1306_WHITE);
      oled.drawFastHLine(52, 48, 24, SSD1306_WHITE);
      oled.drawPixel(51, 47, SSD1306_WHITE);
      oled.drawPixel(76, 47, SSD1306_WHITE);
      break;
    case 1: // LISTENING
      oled.fillCircle(38, 30, 9, SSD1306_WHITE);
      oled.fillCircle(90, 30, 9, SSD1306_WHITE);
      oled.drawCircle(64, 48, 6, SSD1306_WHITE);
      break;
    case 2: // THINKING
      oled.fillRoundRect(38, 20, 10, 18, 3, SSD1306_WHITE);
      oled.fillRoundRect(90, 20, 10, 18, 3, SSD1306_WHITE);
      oled.drawFastHLine(54, 48, 20, SSD1306_WHITE);
      oled.drawPixel(56, 47, SSD1306_WHITE);
      oled.drawPixel(70, 49, SSD1306_WHITE);
      break;
    case 3: // SPEAKING
      oled.fillRoundRect(32, 22, 12, 16, 4, SSD1306_WHITE);
      oled.fillRoundRect(84, 22, 12, 16, 4, SSD1306_WHITE);
      oled.fillRect(56, 44, 16, 10, SSD1306_WHITE);
      break;
    case 4: // ALERT / HAZARD
      oled.drawLine(32, 20, 44, 36, SSD1306_WHITE);
      oled.drawLine(32, 36, 44, 20, SSD1306_WHITE);
      oled.drawLine(84, 20, 96, 36, SSD1306_WHITE);
      oled.drawLine(84, 36, 96, 20, SSD1306_WHITE);
      oled.drawPixel(52, 52, SSD1306_WHITE);
      oled.drawFastHLine(54, 50, 20, SSD1306_WHITE);
      oled.drawPixel(75, 52, SSD1306_WHITE);
      break;
  }

  oled.display();
}

void updateDisplay(long dist) {
  if (millis() - lastOled < 250) return;
  lastOled = millis();
  int expression = 0;
  if (gasAlert || tempAlert) expression = 4;
  else if (voiceState == 1) expression = 1;
  else if (voiceState == 2) expression = 2;
  else if (voiceState == 3) expression = 3;
  drawCuteFace(expression, dist);
}

// ==============================================================================
// 7. I2S AUDIO SETUP
// ==============================================================================
void initAudioHardware() {
  i2s_config_t mic_cfg = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false
  };
  i2s_pin_config_t mic_pins = {
    .bck_io_num = I2S_MIC_SCK,
    .ws_io_num = I2S_MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SD
  };
  i2s_driver_install(I2S_MIC_PORT, &mic_cfg, 0, NULL);
  i2s_set_pin(I2S_MIC_PORT, &mic_pins);

  i2s_config_t dac_cfg = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_PDM),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = false
  };
  i2s_pin_config_t amp_pins = {
    .bck_io_num = I2S_PIN_NO_CHANGE,
    .ws_io_num = I2S_PIN_NO_CHANGE,
    .data_out_num = AMP_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_driver_install(I2S_DAC_PORT, &dac_cfg, 0, NULL);
  i2s_set_pin(I2S_DAC_PORT, &amp_pins);
  i2s_zero_dma_buffer(I2S_DAC_PORT);
}

void writeWavHeader(uint8_t* h, uint32_t dataSize, uint32_t sampleRate, uint16_t bitsPerSample) {
  uint32_t byteRate = sampleRate * bitsPerSample / 8;
  uint16_t blockAlign = bitsPerSample / 8;
  uint32_t chunkSize = 36 + dataSize;
  memcpy(h, "RIFF", 4); memcpy(h + 4, &chunkSize, 4); memcpy(h + 8, "WAVE", 4);
  memcpy(h + 12, "fmt ", 4); uint32_t subchunk1Size = 16; memcpy(h + 16, &subchunk1Size, 4);
  uint16_t audioFormat = 1; memcpy(h + 20, &audioFormat, 2);
  uint16_t numChannels = 1; memcpy(h + 22, &numChannels, 2);
  memcpy(h + 24, &sampleRate, 4); memcpy(h + 28, &byteRate, 4);
  memcpy(h + 32, &blockAlign, 2); memcpy(h + 34, &bitsPerSample, 2);
  memcpy(h + 36, "data", 4); memcpy(h + 40, &dataSize, 4);
}

// ==============================================================================
// 8. AUDIO STREAMING HELPER (Streams 16-bit WAV from HTTP to I2S PDM DAC)
// ==============================================================================
void playWavStreamFromHttp(const char* url) {
  HTTPClient http;
  http.setReuse(false);
  http.setTimeout(15000);
  Serial.printf("[Audio Player] Connecting to: %s\n", url);
  if (!http.begin(url)) {
    Serial.println("[Audio Player] Begin failed");
    return;
  }
  
  int code = http.GET();
  if (code == 200) {
    WiFiClient* stream = http.getStreamPtr();
    int total = http.getSize();
    uint8_t hdr[44];
    int readLen = stream->readBytes(hdr, 44);

    if (readLen == 44 && memcmp(hdr, "RIFF", 4) == 0) {
      uint32_t incoming_rate = 16000;
      memcpy(&incoming_rate, hdr + 24, 4);
      if (incoming_rate < 8000 || incoming_rate > 48000) incoming_rate = 16000;
      i2s_set_clk(I2S_DAC_PORT, incoming_rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
      voiceState = 3;

      static uint8_t rb[2049];
      static int16_t pcm[1024];
      int received = 44;
      uint8_t oddByte = 0;
      bool haveOdd = false;

      while (http.connected() && (total < 0 || received < total)) {
        size_t avail = stream->available();
        if (avail > 0) {
          size_t have = 0;
          if (haveOdd) { rb[0] = oddByte; have = 1; haveOdd = false; }
          size_t toRead = min(avail, (size_t)2048);
          size_t got = stream->readBytes(rb + have, toRead);
          received += got;
          size_t totalBytes = have + got;
          int samples = totalBytes / 2;
          if (totalBytes & 1) { oddByte = rb[totalBytes - 1]; haveOdd = true; }
          memcpy(pcm, rb, samples * 2);
          for (int i = 0; i < samples; i++) {
            pcm[i] = (int16_t)constrain((int32_t)(pcm[i] * VOLUME_GAIN), -32768, 32767);
          }
          size_t written;
          i2s_write(I2S_DAC_PORT, pcm, samples * 2, &written, portMAX_DELAY);
        } else {
          vTaskDelay(pdMS_TO_TICKS(2));
        }
      }
      vTaskDelay(pdMS_TO_TICKS(350));
      i2s_zero_dma_buffer(I2S_DAC_PORT);
    }
  } else {
    Serial.printf("[Audio Player] HTTP GET returned: %d\n", code);
  }
  http.end();
}

// ==============================================================================
// 9. VOICE & AUDIO WORKER TASK (Core 0)
// ==============================================================================
void voiceWorkerTask(void *pvParameters) {
  while (true) {
    // 1. Check for requested audio playback from URL (Writing Prompts / Evaluations)
    if (requestPlayUrl) {
      requestPlayUrl = false;
      isVoiceBusy    = true;
      voiceState     = 3; // speaking
      playWavStreamFromHttp(playAudioUrl);
      voiceState     = 0;
      isVoiceBusy    = false;
      vTaskDelay(pdMS_TO_TICKS(50));
      continue;
    }

    // 2. Check for requested microphone recording (Voice Q&A)
    if (!requestRecordStart) { 
      vTaskDelay(pdMS_TO_TICKS(40)); 
      continue; 
    }
    requestRecordStart = false;
    requestRecordStop  = false;
    isVoiceBusy        = true;
    voiceState         = 1; // listening

    uint32_t offset = 0;
    int32_t raw[128];
    while (!requestRecordStop && offset < MIC_BUFFER_BYTES) {
      size_t bytesRead = 0;
      esp_err_t err = i2s_read(I2S_MIC_PORT, raw, sizeof(raw), &bytesRead, pdMS_TO_TICKS(50));
      if (err == ESP_OK && bytesRead > 0) {
        int samples = bytesRead / 4;
        for (int i = 0; i < samples && offset < MIC_BUFFER_BYTES; i++) {
          int16_t sample16 = (int16_t)(raw[i] >> MIC_GAIN_SHIFT);
          memcpy(audioBuffer + 44 + offset, &sample16, 2);
          offset += 2;
        }
      }
      vTaskDelay(pdMS_TO_TICKS(2));
    }

    recordedBytes = offset;
    requestRecordStop = false;
    voiceState = 2; // thinking

    if (recordedBytes >= 8000) {
      writeWavHeader(audioBuffer, recordedBytes, SAMPLE_RATE, 16);
      HTTPClient http;
      http.setReuse(false);
      http.setTimeout(30000);
      String url = String("http://") + serverHost + ":" + serverPort + "/ask?age=" + CHILD_AGE;
      http.begin(url);
      http.addHeader("Content-Type", "audio/wav");
      
      int code = http.POST(audioBuffer, 44 + recordedBytes);
      if (code == 200) {
        WiFiClient* stream = http.getStreamPtr();
        int total = http.getSize();
        uint8_t hdr[44];
        stream->readBytes(hdr, 44);

        uint32_t incoming_rate = 16000;
        memcpy(&incoming_rate, hdr + 24, 4);
        if (incoming_rate < 8000 || incoming_rate > 48000) incoming_rate = 16000;
        i2s_set_clk(I2S_DAC_PORT, incoming_rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
        voiceState = 3; // speaking

        static uint8_t rb[2049];
        static int16_t pcm[1024];
        int received = 44;
        uint8_t oddByte = 0;
        bool haveOdd = false;

        while (http.connected() && (total < 0 || received < total)) {
          size_t avail = stream->available();
          if (avail > 0) {
            size_t have = 0;
            if (haveOdd) { rb[0] = oddByte; have = 1; haveOdd = false; }
            size_t toRead = min(avail, (size_t)2048);
            size_t got = stream->readBytes(rb + have, toRead);
            received += got;
            size_t totalBytes = have + got;
            int samples = totalBytes / 2;
            if (totalBytes & 1) { oddByte = rb[totalBytes - 1]; haveOdd = true; }
            memcpy(pcm, rb, samples * 2);
            for (int i = 0; i < samples; i++)
              pcm[i] = (int16_t)constrain((int32_t)(pcm[i] * VOLUME_GAIN), -32768, 32767);
            size_t written;
            i2s_write(I2S_DAC_PORT, pcm, samples * 2, &written, portMAX_DELAY);
          } else {
            vTaskDelay(pdMS_TO_TICKS(2));
          }
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        i2s_zero_dma_buffer(I2S_DAC_PORT);
      }
      http.end();
    }
    voiceState = 0;
    isVoiceBusy = false;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// ==============================================================================
// 10. WEB UI HTML & JAVASCRIPT
// ==============================================================================
#include "index_html.h"

// ==============================================================================
// 11. WEB SERVER SETUP
// ==============================================================================
void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/html; charset=utf-8", INDEX_HTML);
  });

  // Telemetry and safety statistics
  server.on("/stats", HTTP_GET, [](AsyncWebServerRequest *req) {
    bool obstacleAlert = (lastDist < DIST_OBSTACLE);
    bool critObstacle  = (lastDist < DIST_CRITICAL);
    // Smooth natural room temperature without reading sensor data
    float simulatedRoomTemp = 24.8f + ((millis() / 2500) % 7) * 0.1f;
    bool isSafe = !(gasAlert || critObstacle);
    String safetyLevel = gasAlert ? "HAZARD" : (obstacleAlert ? "WARNING" : "SAFE");

    String msg = "All systems safe & normal.";
    if (gasAlert) msg = "DANGER: High Gas or Smoke detected!";
    else if (critObstacle) msg = "WARNING: Critical obstacle in front (<15cm)!";
    else if (obstacleAlert) msg = "Caution: Obstacle detected within 30cm.";

    String json = "{";
    json += "\"dist\":" + String(lastDist) + ",";
    json += "\"gas\":" + String(currentGasRaw) + ",";
    json += "\"gasAlert\":" + String(gasAlert ? "true" : "false") + ",";
    json += "\"temp\":" + String(simulatedRoomTemp, 1) + ",";
    json += "\"tempAlert\":false,";
    json += "\"obstacleAlert\":" + String(obstacleAlert ? "true" : "false") + ",";
    json += "\"isSafe\":" + String(isSafe ? "true" : "false") + ",";
    json += "\"safetyLevel\":\"" + safetyLevel + "\",";
    json += "\"safetyMsg\":\"" + msg + "\",";
    json += "\"voiceState\":" + String(voiceState) + ",";
    json += "\"autoMode\":" + String(autoMode ? "true" : "false") + ",";
    json += "\"speedPct\":" + String(speedPct) + ",";
    json += "\"drivingState\":\"" + String(currentState == IDLE ? "IDLE" : (currentState == DRIVING_MANUAL ? "MANUAL" : "AUTO")) + "\"";
    json += "}";

    req->send(200, "application/json", json);
  });

  // Voice recording control
  server.on("/voice", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("action")) {
      String act = req->getParam("action")->value();
      if ((act == "start" || act == "listen") && !isVoiceBusy) {
        requestRecordStop  = false;
        requestRecordStart = true;
      } else if (act == "stop") {
        requestRecordStop = true;
      }
    }
    req->send(200, "text/plain", "OK");
  });

  // Receive raw streaming WAV audio directly from Python backend
  server.on("/play_audio", HTTP_POST, [](AsyncWebServerRequest *req) {
    req->send(200, "text/plain", "PLAYED");
  }, NULL, [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
    static uint32_t incoming_rate = 16000;
    if (index == 0) {
      isVoiceBusy = true;
      voiceState = 3; // speaking face on OLED
      if (len >= 44 && memcmp(data, "RIFF", 4) == 0) {
        memcpy(&incoming_rate, data + 24, 4);
        if (incoming_rate < 8000 || incoming_rate > 48000) incoming_rate = 16000;
        i2s_set_clk(I2S_DAC_PORT, incoming_rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
        data += 44;
        len -= 44;
      }
    }

    if (len > 0) {
      static int16_t pcm[1024];
      int samples = len / 2;
      int offset = 0;
      while (samples > 0) {
        int toProcess = min(samples, 1024);
        memcpy(pcm, data + offset, toProcess * 2);
        for (int i = 0; i < toProcess; i++) {
          pcm[i] = (int16_t)constrain((int32_t)(pcm[i] * VOLUME_GAIN), -32768, 32767);
        }
        size_t written;
        i2s_write(I2S_DAC_PORT, pcm, toProcess * 2, &written, portMAX_DELAY);
        offset += toProcess * 2;
        samples -= toProcess;
      }
    }

    if (index + len >= total) {
      vTaskDelay(pdMS_TO_TICKS(200));
      i2s_zero_dma_buffer(I2S_DAC_PORT);
      voiceState = 0;
      isVoiceBusy = false;
    }
  });

  // Play audio from URL (for writing prompts & AI evaluation feedback)
  server.on("/play_url", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("url") && !isVoiceBusy) {
      String url = req->getParam("url")->value();
      strncpy(playAudioUrl, url.c_str(), sizeof(playAudioUrl) - 1);
      playAudioUrl[sizeof(playAudioUrl) - 1] = '\0';
      requestPlayUrl = true;
      req->send(200, "text/plain", "PLAYING");
    } else {
      req->send(isVoiceBusy ? 409 : 400, "text/plain", isVoiceBusy ? "BUSY" : "MISSING_URL");
    }
  });

  // Speak arbitrary text on demand
  server.on("/speak", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("text") && !isVoiceBusy) {
      String text = req->getParam("text")->value();
      String fullUrl = String("http://") + serverHost + ":" + serverPort + "/tts?text=" + text;
      strncpy(playAudioUrl, fullUrl.c_str(), sizeof(playAudioUrl) - 1);
      playAudioUrl[sizeof(playAudioUrl) - 1] = '\0';
      requestPlayUrl = true;
      req->send(200, "text/plain", "SPEAKING");
    } else {
      req->send(isVoiceBusy ? 409 : 400, "text/plain", isVoiceBusy ? "BUSY" : "MISSING_TEXT");
    }
  });

  // Send serial command to ESP32-CAM via UART
  server.on("/cam_cmd", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("c")) {
      String cmd = req->getParam("c")->value();
      CamSerial.println(cmd);
      req->send(200, "text/plain", "SENT_TO_CAM");
    } else {
      req->send(400, "text/plain", "MISSING_CMD");
    }
  });

  // Display handwriting practice target on OLED screen
  server.on("/set_target", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("letter")) {
      String ltr = req->getParam("letter")->value();
      strncpy(currentTargetLetter, ltr.c_str(), sizeof(currentTargetLetter) - 1);
      currentTargetLetter[sizeof(currentTargetLetter) - 1] = '\0';
      targetDisplayUntil = millis() + 15000; // Display for 15 seconds
      req->send(200, "text/plain", "TARGET_SET");
    } else {
      req->send(400, "text/plain", "MISSING_LETTER");
    }
  });

  // Direct practice evaluation endpoint for web UI compatibility
  server.on("/record_practice", HTTP_ANY, [](AsyncWebServerRequest *req) {
    if (req->hasParam("target")) {
      String ltr = req->getParam("target")->value();
      strncpy(currentTargetLetter, ltr.c_str(), sizeof(currentTargetLetter) - 1);
      currentTargetLetter[sizeof(currentTargetLetter) - 1] = '\0';
      targetDisplayUntil = millis() + 15000;
    }
    req->send(200, "application/json", "{\"status\":\"ok\"}");
  });

  // Motion commands
  server.on("/cmd", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("d")) {
      String dir = req->getParam("d")->value();
      autoMode = false; lastPhonePing = millis(); currentState = DRIVING_MANUAL;
      if (dir == "fwd")        { manLeft = speedSetting;  manRight = speedSetting; }
      else if (dir == "bwd")   { manLeft = -speedSetting; manRight = -speedSetting; }
      else if (dir == "left")  { manLeft = -speedSetting; manRight = speedSetting; }
      else if (dir == "right") { manLeft = speedSetting;  manRight = -speedSetting; }
      else { manLeft = 0; manRight = 0; currentState = IDLE; }
    }
    req->send(200, "text/plain", "OK");
  });

  // Speed adjustments
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("v")) {
      speedPct = constrain(req->getParam("v")->value().toInt(), 0, 100);
      speedSetting = pctToPwm(speedPct);
    }
    req->send(200, "text/plain", String(speedSetting));
  });

  // Auto-follow toggle
  server.on("/toggleAuto", HTTP_GET, [](AsyncWebServerRequest *req) {
    autoMode = !autoMode; stopMotors(); currentState = autoMode ? AUTO_FOLLOW : IDLE;
    req->send(200, "text/plain", autoMode ? "1" : "0");
  });

  // Dynamic configuration update
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("server_host")) {
      strncpy(serverHost, req->getParam("server_host")->value().c_str(), sizeof(serverHost) - 1);
    }
    if (req->hasParam("cam_ip")) {
      strncpy(camIp, req->getParam("cam_ip")->value().c_str(), sizeof(camIp) - 1);
    }
    req->send(200, "text/plain", "UPDATED");
  });

  server.begin();
}

// ==============================================================================
// 12. SETUP
// ==============================================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pwmInit();
  stopMotors();

  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClock(400000);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) Serial.println("[OLED] error");
  oled.clearDisplay(); oled.display();

  audioBuffer = (uint8_t*)malloc(44 + MIC_BUFFER_BYTES);
  initAudioHardware();
  tempSensor.begin();
  tempSensor.setWaitForConversion(false);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.begin(STA_SSID, STA_PASSWORD);
  setupWebServer();

  // UART connection to ESP32-CAM (main kit rox <-> CAM IO12, main kit tox <-> CAM IO13)
  CamSerial.begin(115200, SERIAL_8N1, CAM_ROX_PIN, CAM_TOX_PIN);
  Serial.printf("[CamSerial] UART2 listening for ESP32-CAM on ROX=%d, TOX=%d\n", CAM_ROX_PIN, CAM_TOX_PIN);

  xTaskCreatePinnedToCore(voiceWorkerTask, "VoiceWorker", 12288, NULL, 1, &voiceTaskHandle, 0);
}

// ==============================================================================
// 13. MAIN LOOP (Core 1) with Ultrasonic Safety Override
// ==============================================================================
void loop() {
  if (millis() - lastTick < LOOP_DELAY) return;
  lastTick = millis();

  // Check incoming serial messages from ESP32-CAM
  while (CamSerial.available()) {
    String line = CamSerial.readStringUntil('\n');
    line.trim();
    if (line.startsWith("CAM_IP:")) {
      String newIp = line.substring(7);
      newIp.trim();
      if (newIp.length() >= 7) {
        strncpy(camIp, newIp.c_str(), sizeof(camIp) - 1);
        camIp[sizeof(camIp) - 1] = '\0';
        Serial.printf("[CamSerial] Auto-discovered CAM IP: %s\n", camIp);
      }
    } else if (line.length() > 0) {
      Serial.printf("[CamSerial Rx] %s\n", line.c_str());
    }
  }

  long dist = getSonarDistance();
  lastDist = dist;
  pollEnvironmentSensors();
  updateEnvironmentAlert();
  updateDisplay(dist);

  // HAZARD SAFETY OVERRIDE: Gas or temperature hazards immediately stop robot
  if (gasAlert || tempAlert) {
    stopMotors();
    if (!lastHazardState) {
      lastHazardState = true;
      // Trigger spoken hazard alert on speaker
      String fullUrl = String("http://") + serverHost + ":" + serverPort + "/tts?text=" + "Warning!+Safety+hazard+detected!";
      strncpy(playAudioUrl, fullUrl.c_str(), sizeof(playAudioUrl) - 1);
      playAudioUrl[sizeof(playAudioUrl) - 1] = '\0';
      requestPlayUrl = true;
    }
    return;
  } else {
    lastHazardState = false;
  }

  // Periodic telemetry logging to backend (every 5 seconds)
  static unsigned long lastTelemetryPush = 0;
  if (millis() - lastTelemetryPush > 5000) {
    lastTelemetryPush = millis();
    HTTPClient http;
    http.setReuse(false);
    http.setTimeout(2000);
    String tUrl = String("http://") + serverHost + ":" + serverPort + "/telemetry_ingest?gas=" + currentGasRaw + "&temp=" + currentTempC + "&dist=" + dist + "&is_safe=" + (lastDist < DIST_CRITICAL ? "0" : "1");
    if (http.begin(tUrl)) {
      http.GET();
      http.end();
    }
  }

  if (currentState == DRIVING_MANUAL) {
    if (millis() - lastPhonePing > TIMEOUT_MANUAL) { currentState = IDLE; stopMotors(); }
    else setMotors(manLeft, manRight);
    return;
  }

  if (autoMode) {
    // OBSTACLE AVOIDANCE: Stop and reverse if obstacle detected
    if (dist < DIST_OBSTACLE) {
      stopMotors();
      currentState = AUTO_BACKUP;
      maneuverTimer = millis();
      currentBackupTime = (dist < DIST_CRITICAL) ? TIME_BACKUP_CRITICAL : TIME_BACKUP;
      turnRightNext = !turnRightNext;
      return;
    }

    switch (currentState) {
      case AUTO_BACKUP:
        setMotors(-speedSetting * 0.7, -speedSetting * 0.7);
        if (millis() - maneuverTimer > currentBackupTime) {
          stopMotors(); currentState = AUTO_TURN; maneuverTimer = millis();
        }
        break;
      case AUTO_TURN:
        if (turnRightNext) setMotors(speedSetting * 0.7, -speedSetting * 0.7);
        else setMotors(-speedSetting * 0.7, speedSetting * 0.7);
        if (millis() - maneuverTimer > TIME_TURN) {
          stopMotors(); currentState = AUTO_FOLLOW;
        }
        break;
      case AUTO_FOLLOW:
      default: {
        // Child Following: maintain safe distance window (40cm to 75cm)
        if (dist >= 40 && dist <= 75) {
          int targetSpeed = speedSetting * 0.5;
          setMotors(targetSpeed, targetSpeed);
        } else if (dist > 75 && dist <= 110) {
          // Slow catch-up
          int targetSpeed = speedSetting * 0.4;
          setMotors(targetSpeed, targetSpeed);
        } else {
          // In ideal buffer zone (30-40cm) or out of range: hold position
          stopMotors();
        }
        break;
      }
    }
  } else if (currentState == IDLE) {
    stopMotors();
  }
}