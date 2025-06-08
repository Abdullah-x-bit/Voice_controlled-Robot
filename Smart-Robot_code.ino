#include <ESP8266WiFi.h>                // Include Wi-Fi library for ESP8266
#include <Firebase_ESP_Client.h>        // Include Firebase client library
#include "addons/RTDBHelper.h"          // Firebase helper (Real-Time DB)
#include "addons/TokenHelper.h"         // Firebase helper (Token mgmt)

// ---------- Wi-Fi Credentials ----------
#define WIFI_SSID       "ZC-Guest"          // Replace with your Wi-Fi SSID
#define WIFI_PASSWORD   "Guest&2024@ZC1"          // Replace with your Wi-Fi password

// ---------- Firebase Credentials ----------
#define API_KEY         "AIzaSyDI7ECvv_IHSc3-2rDe5x1vSLt1thtdoak"          // Replace with your Firebase API key
#define DATABASE_URL    "https://robot-614df-default-rtdb.firebaseio.com/"  // Your Firebase Realtime DB URL

// ---------- Firebase Objects ----------
FirebaseData fbdo;                      // Firebase data object
FirebaseAuth auth;                      // Firebase authentication
FirebaseConfig config;                  // Firebase config
bool signupOK = false;                  // Track sign-up status

// ---------- Motor Control Pins ----------
#define PWM_L D1   // Left motor PWM
#define IN1   D2   // Left motor direction 1
#define IN2   D3   // Left motor direction 2

#define PWM_R D5   // Right motor PWM
#define IN3   D0   // Right motor direction 1
#define IN4   D4   // Right motor direction 2

// ---------- Motor Speed Variables ----------
int leftSpeed = 0;
int rightSpeed = 0;

void setup() {
  Serial.begin(9600);  // Start serial communication for debugging

  // Set motor control pins as output
  pinMode(PWM_L, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(PWM_R, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  connectToWiFi();     // Connect to Wi-Fi
  initFirebase();      // Initialize Firebase
}

void loop() {
  // Check if Firebase is ready and signed in
  if (Firebase.ready() && signupOK) {

    // --------- Read Left Wheel Speed from Firebase ---------
    if (Firebase.RTDB.getString(&fbdo, "/SmartRobotApp/Lwheel_speed")) {
      String L_str = fbdo.stringData();      // Get string value
      leftSpeed = L_str.toInt();             // Convert to integer
      Serial.print("Left Speed: ");
      Serial.println(leftSpeed);
    }

    // --------- Read Right Wheel Speed from Firebase ---------
    if (Firebase.RTDB.getString(&fbdo, "/SmartRobotApp/Rwheel_speed")) {
      String R_str = fbdo.stringData();      // Get string value
      rightSpeed = R_str.toInt();            // Convert to integer
      Serial.print("Right Speed: ");
      Serial.println(rightSpeed);
    }

    // --------- Apply Motor Control ---------
    controlMotor(leftSpeed, PWM_L, IN1, IN2);  // Control left motor
    controlMotor(rightSpeed, PWM_R, IN3, IN4); // Control right motor
  }

  delay(200);  // Wait to avoid overloading Firebase
}

// ---------- Connect to Wi-Fi ----------
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Start Wi-Fi connection
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ---------- Initialize Firebase ----------
void initFirebase() {
  config.api_key = API_KEY;              // Set Firebase API key
  config.database_url = DATABASE_URL;    // Set Firebase DB URL

  // Sign up anonymously
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign-up successful");
    signupOK = true;
  } else {
    Serial.printf("Firebase sign-up failed: %s\n", config.signer.signupError.message.c_str());
  }

  // Enable token auto-refresh
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);          // Auto reconnect if WiFi drops
}

// ---------- Motor Control Function ----------
// Takes a speed value (positive or negative), the PWM pin, and two direction pins
void controlMotor(int speed, int pwmPin, int inPin1, int inPin2) {
  int absSpeed = abs(speed);             // Get absolute value of speed
  if (absSpeed > 255) absSpeed = 255;    // Limit PWM to max 255

  analogWrite(pwmPin, absSpeed);         // Set motor speed

  if (speed > 0) {
    digitalWrite(inPin1, HIGH);          // Forward
    digitalWrite(inPin2, LOW);
  } else if (speed < 0) {
    digitalWrite(inPin1, LOW);           // Backward
    digitalWrite(inPin2, HIGH);
  } else {
    digitalWrite(inPin1, LOW);           // Stop
    digitalWrite(inPin2, LOW);
  }
}