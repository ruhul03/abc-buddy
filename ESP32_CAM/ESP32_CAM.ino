/*
  ABC Buddy — ESP32-CAM Dedicated Firmware
  =========================================
  Hardware UART Pinout:
  - ESP32-CAM IO12 (TX) -> Connected to Main Kit rox
  - ESP32-CAM IO13 (RX) -> Connected to Main Kit tox

  Features:
  - OV2640 Camera Capture (:80/capture)
  - Auto-Discovery: Sends its Wi-Fi IP address over UART to Main ESP32 ("CAM_IP:192.168.x.x\n")
  - Hardware Command Execution over Serial (SNAP, PING, FLASH_ON, FLASH_OFF)
*/

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include "esp_timer.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Wi-Fi Configuration
const char* ssid = "GB_Boysian";
const char* password = "01768833742";

// Hardware UART pins connected to Main Kit
#define CAM_TX_PIN 12 // Connected to Main Kit rox
#define CAM_RX_PIN 13 // Connected to Main Kit tox
#define FLASH_LED_PIN 4

// AI-Thinker Camera Pin Map
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

httpd_handle_t camera_httpd = NULL;

static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }
  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  esp_err_t res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
  return res;
}

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t capture_uri = {
    .uri       = "/capture",
    .method    = HTTP_GET,
    .handler   = capture_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &capture_uri);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector

  Serial.begin(115200);

  // Serial2 link to Main Kit: RX=13, TX=12
  Serial2.begin(115200, SERIAL_8N1, CAM_RX_PIN, CAM_TX_PIN);

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("Camera IP: ");
  Serial.println(WiFi.localIP());

  startCameraServer();

  // Send IP to Main ESP32 kit over Serial2
  Serial2.printf("CAM_IP:%s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // Check for commands from Main ESP32 kit
  if (Serial2.available()) {
    String cmd = Serial2.readStringUntil('\n');
    cmd.trim();
    if (cmd == "SNAP") {
      digitalWrite(FLASH_LED_PIN, HIGH);
      delay(80);
      digitalWrite(FLASH_LED_PIN, LOW);
      Serial2.println("SNAP_OK");
    } else if (cmd == "PING") {
      Serial2.printf("CAM_IP:%s\n", WiFi.localIP().toString().c_str());
    } else if (cmd == "FLASH_ON") {
      digitalWrite(FLASH_LED_PIN, HIGH);
    } else if (cmd == "FLASH_OFF") {
      digitalWrite(FLASH_LED_PIN, LOW);
    }
  }

  // Periodic heartbeat / IP broadcast every 10 seconds
  static unsigned long lastIpBroadcast = 0;
  if (millis() - lastIpBroadcast > 10000) {
    lastIpBroadcast = millis();
    Serial2.printf("CAM_IP:%s\n", WiFi.localIP().toString().c_str());
  }

  delay(10);
}
