
# Smart Robot Motor Control with ESP8266 and Firebase Realtime Database

This project demonstrates how to control a two-wheel smart robot's motors remotely using an ESP8266 microcontroller connected to Firebase Realtime Database. The ESP8266 fetches speed commands from Firebase and adjusts the motor speeds accordingly.

---

## Features

- Connects ESP8266 to Wi-Fi network.
- Authenticates anonymously with Firebase.
- Reads left and right motor speed values from Firebase Realtime Database.
- Controls two DC motors (left and right wheels) with direction and speed control via PWM.
- Supports forward, backward, and stop motor states.
- Automatically reconnects to Wi-Fi and Firebase if connection drops.

---

## Hardware Requirements

- ESP8266 development board (e.g., NodeMCU, Wemos D1 Mini)
- Two DC motors with motor driver (e.g., L298N or similar)
- Appropriate power supply for motors and ESP8266
- Connecting wires and breadboard or PCB

---

## Wiring

| ESP8266 Pin | Motor Driver Pin         | Description             |
|-------------|-------------------------|-------------------------|
| D1          | PWM Left Motor           | PWM speed control left  |
| D2          | IN1 Left Motor           | Direction control left  |
| D3          | IN2 Left Motor           | Direction control left  |
| D5          | PWM Right Motor          | PWM speed control right |
| D0          | IN3 Right Motor          | Direction control right |
| D4          | IN4 Right Motor          | Direction control right |

---

## Firebase Setup

1. Create a Firebase project at [Firebase Console](https://console.firebase.google.com/).
2. Enable Realtime Database and get the database URL.
3. Generate a Web API key from the project settings.
4. Structure your Realtime Database like this:
    ```json
    {
      "SmartRobotApp": {
        "Lwheel_speed": "0",
        "Rwheel_speed": "0"
      }
    }
    ```
5. Update `API_KEY` and `DATABASE_URL` constants in the code with your Firebase project credentials.

---

## Code Overview

- `connectToWiFi()`: Connects ESP8266 to your Wi-Fi network.
- `initFirebase()`: Authenticates ESP8266 with Firebase anonymously and initializes Firebase services.
- `loop()`: Continuously reads motor speed commands from Firebase and applies them to motors.
- `controlMotor()`: Controls motor speed and direction via PWM and direction pins.

---

## Usage

1. Flash the code to your ESP8266 board.
2. Make sure your Firebase Realtime Database has the motor speed values under `/SmartRobotApp/Lwheel_speed` and `/SmartRobotApp/Rwheel_speed`.
3. Update the speed values remotely via Firebase console or any Firebase client app.
4. The ESP8266 will read these values and control the motors accordingly.

---

## Dependencies

- [ESP8266WiFi library](https://github.com/esp8266/Arduino)
- [Firebase ESP Client library](https://github.com/FirebaseExtended/firebase-esp-client)

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Author

Abdullah Afifi
