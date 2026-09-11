/*
  AURA — Autonomous Unified Rover Assistant
  ESP32-CAM firmware: WiFi camera streaming + pan/tilt servo control over WebSocket

  Hardware: ESP32-CAM (AI-Thinker), 2x servo motors (pan/tilt)
  Libraries required (Arduino IDE Library Manager):
    - WiFi.h            (bundled with ESP32 board package)
    - esp_camera.h       (bundled with ESP32 board package)
    - ESPAsyncWebServer
    - WebSocketsServer  (arduinoWebSockets by Links2004)
    - ESP32Servo

  Before uploading: set your WiFi credentials below.
*/

#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Servo Pins
#define PAN_SERVO 12
#define TILT_SERVO 13

Servo panServo;
Servo tiltServo;

// Web Server & WebSocket
AsyncWebServer server(80);
WebSocketsServer webSocket(81);

// Camera Pins (AI Thinker module pinout)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Camera Setup
void startCamera() {
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

  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }
}

// WebSocket Command Handler — pan/tilt servo control
void handleWebSocketMessage(uint8_t *payload) {
  String msg = String((char *)payload);

  if (msg == "pan_left") panServo.write(panServo.read() - 10);
  else if (msg == "pan_right") panServo.write(panServo.read() + 10);
  else if (msg == "tilt_up") tiltServo.write(tiltServo.read() - 10);
  else if (msg == "tilt_down") tiltServo.write(tiltServo.read() + 10);
}

// WebSocket Event
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    handleWebSocketMessage(payload);
  }
}

// Setup
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println("WiFi Connected");

  panServo.attach(PAN_SERVO);
  tiltServo.attach(TILT_SERVO);

  startCamera();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.begin();
}

// Loop
void loop() {
  webSocket.loop();
}
