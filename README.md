# AURA — Autonomous Unified Rover Assistant

A WiFi-controlled 4-wheeled robotic vehicle with a detachable 360° camera module for live video streaming and two-way audio communication. Designed for dual use: mobile surveillance as a rover, or static wall-mounted room monitoring — switching between the two by detaching the same camera module.

## Overview

The system pairs an ESP32-CAM (handling video capture, WiFi, and pan/tilt camera control) with an L298N-driven 4-motor chassis, controlled in real time over WebSocket from a browser-based interface. A detachable hemisphere-shaped camera enclosure with a quick lock/unlock mechanism lets the same module run in two physical configurations without separate hardware.

## Objectives

- WiFi-controlled robotic car with real-time movement commands
- Real-time video streaming from an onboard camera
- Two-way audio communication (listen to and speak into the remote environment)
- Full 360° camera rotation (pan + tilt)
- A single detachable camera module usable in both vehicle-mounted and wall-mounted modes

## Key features

- **WiFi-based control** — driven from any mobile browser/web interface; forward/backward/left/right plus independent camera pan & tilt
- **360° camera rotation** — pan and tilt servo motors give full horizontal rotation and vertical adjustment
- **Detachable hemisphere camera module** — dome-shaped enclosure with a quick lock/unlock mechanism and plug-and-play connectors; swaps between vehicle-mounted and wall-mounted modes
- **Live video streaming** — ESP32-CAM streams over a local WiFi server (MJPEG); architecture allows extending to YouTube/Facebook Live via RTMP
- **Two-way communication** — INMP441 microphone + PAM8610 amplifier for audio in both directions
- **Dual functionality** — same hardware serves as a static room-monitoring camera or an active mobile rover

## Hardware

| Component | Function |
|---|---|
| ESP32-CAM | Video processing & WiFi streaming |
| L298N Motor Driver | Drives the 4 DC motors |
| DC Motors ×4 | Vehicle movement |
| Servo Motors ×2 | Camera pan & tilt |
| INMP441 (mic) | Audio input |
| PAM8610 (amplifier) | Audio output |
| Li-ion Batteries (3× 3000mAh) | Power supply |
| XL4005 converter | Voltage regulation |

## Software / firmware

- Arduino IDE
- ESP32 libraries: `WiFi.h`, `esp_camera.h`, `ESPAsyncWebServer`, `WebSocketsServer` (arduinoWebSockets), `ESP32Servo`
- Firmware entry point: `AURA_firmware.ino`

## Circuit design

**Power:** batteries → XL4005 buck converter → regulated output, with separate supply lines for the motors and the ESP32-CAM (keeps motor current draw from browning out the camera/WiFi).

**Motor control:** ESP32 sends drive signals to the L298N, which powers the 4 DC motors.

**Camera + servo:** the ESP32-CAM module handles video capture directly; GPIO 12 and 13 drive the pan and tilt servos.

## Working principle

1. ESP32 connects to WiFi and starts the camera
2. An async web server + WebSocket server come up on the device
3. User opens the control interface in a browser
4. Movement/camera commands are sent over HTTP/WebSocket
5. The vehicle moves and the camera streams live video in response
6. Pan/tilt commands rotate the camera; audio streams both directions

## Run it

1. Open `AURA_firmware.ino` in Arduino IDE with the ESP32 board package installed
2. Install the required libraries via Library Manager (see above)
3. Set `ssid` and `password` in the firmware to your network credentials
4. Select your ESP32-CAM board variant (AI-Thinker) and upload
5. Open the Serial Monitor to confirm WiFi connection and note the assigned IP
6. Connect to the device's WebSocket interface (port 81) to send `pan_left` / `pan_right` / `tilt_up` / `tilt_down` commands; video streams from the camera server on port 80

## Applications

- Security surveillance
- Home monitoring
- Remote inspection robot
- Live event streaming
- Robotics research

## Advantages

- Low cost, fully wireless control, portable, dual-mode (mobile + static), real-time two-way communication

## Limitations

- Battery life is limited by the onboard Li-ion pack
- Range is bounded by WiFi coverage
- Streaming quality is capped by the ESP32-CAM's hardware (VGA/JPEG)

## Future enhancements

- Onboard AI object detection
- 4G/5G streaming module for range independence
- Voice command control
- Solar charging
- GPS tracking

## Notes

This repository documents the vehicle-movement and motor-driver side of the system (chassis, L298N, DC motors) alongside the camera/streaming firmware; the movement command handling on the ESP32 side follows the same WebSocket pattern shown in `AURA_firmware.ino` for pan/tilt, extended to drive the L298N motor driver.
