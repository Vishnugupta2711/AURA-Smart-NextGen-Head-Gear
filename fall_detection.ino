#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

Adafruit_MPU6050 mpu;

// Pin for the buzzer
const int BUZZER_PIN = 15; // D8 on NodeMCU

// Access Point credentials
const char* ap_ssid = "ESP_Fall_Detection";   // Set your desired SSID here
const char* ap_password = "12345678";         // Set your desired password here

ESP8266WebServer server(80); // Web server on port 80

// Fall detection status
String fallStatus = "No fall detected";

// Function to handle root webpage
void handleRoot() {
  String html = "<!DOCTYPE html>\
<html>\
<head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Fall Detection System</title>\
    <style>\
        body {\
            font-family: 'Arial', sans-serif;\
            margin: 0;\
            padding: 20px;\
            background: #f0f2f5;\
            display: flex;\
            flex-direction: column;\
            align-items: center;\
            min-height: 100vh;\
        }\
        .container {\
            max-width: 800px;\
            width: 100%;\
            text-align: center;\
        }\
        .status-card {\
            background: white;\
            border-radius: 15px;\
            padding: 20px;\
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);\
            margin: 20px 0;\
            position: relative;\
            overflow: hidden;\
        }\
        .status-indicator {\
            width: 100px;\
            height: 100px;\
            border-radius: 50%;\
            margin: 20px auto;\
            display: flex;\
            align-items: center;\
            justify-content: center;\
            font-size: 24px;\
            color: white;\
            position: relative;\
        }\
        @keyframes pulse {\
            0% { transform: scale(1); opacity: 1; }\
            50% { transform: scale(1.1); opacity: 0.8; }\
            100% { transform: scale(1); opacity: 1; }\
        }\
        @keyframes shake {\
            0%, 100% { transform: translateX(0); }\
            25% { transform: translateX(-5px); }\
            75% { transform: translateX(5px); }\
        }\
        .status-safe {\
            background: #4CAF50;\
            animation: pulse 2s infinite;\
        }\
        .status-danger {\
            background: #f44336;\
            animation: shake 0.5s infinite;\
        }\
        .location-btn {\
            background: #2196F3;\
            color: white;\
            padding: 12px 24px;\
            border: none;\
            border-radius: 25px;\
            cursor: pointer;\
            font-size: 16px;\
            margin: 20px 0;\
            transition: transform 0.3s ease;\
        }\
        .location-btn:hover {\
            transform: translateY(-2px);\
        }\
        @keyframes slideIn {\
            from { transform: translateY(20px); opacity: 0; }\
            to { transform: translateY(0); opacity: 1; }\
        }\
        .slide-in {\
            animation: slideIn 0.5s ease-out;\
        }\
        .readings {\
            display: grid;\
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));\
            gap: 20px;\
            margin: 20px 0;\
        }\
        .reading-card {\
            background: white;\
            padding: 15px;\
            border-radius: 10px;\
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);\
        }\
        .map-container {\
            width: 100%;\
            height: 300px;\
            border-radius: 15px;\
            overflow: hidden;\
            margin: 20px 0;\
        }\
    </style>\
</head>\
<body>\
    <div class='container slide-in'>\
        <h1>Fall Detection System</h1>\
        <div class='status-card'>\
            <h2>Current Status</h2>\
            <div id='statusIndicator' class='status-indicator status-safe'>\
                <span id='statusText'>Safe</span>\
            </div>\
        </div>\
        <a href='https://maps.app.goo.gl/ZZ9tw4nBL8verG769' target='_blank' class='location-btn'>\
            📍 Share Current Location\
        </a>\
        <div class='readings'>\
            <div class='reading-card'>\
                <h3>Last Update</h3>\
                <p id='timestamp'></p>\
            </div>\
            <div class='reading-card'>\
                <h3>Device ID</h3>\
                <p>ESP8266_FD1</p>\
            </div>\
        </div>\
    </div>\
    <script>\
        function updateStatus(status) {\
            const indicator = document.getElementById('statusIndicator');\
            const statusText = document.getElementById('statusText');\
            if (status.includes('Fall detected')) {\
                indicator.className = 'status-indicator status-danger';\
                statusText.textContent = '⚠️ FALL';\
            } else {\
                indicator.className = 'status-indicator status-safe';\
                statusText.textContent = '✓ Safe';\
            }\
        }\
        function updateTimestamp() {\
            const now = new Date();\
            document.getElementById('timestamp').textContent = now.toLocaleTimeString();\
        }\
        setInterval(updateTimestamp, 1000);\
        updateTimestamp();\
        updateStatus('" + fallStatus + "');\
    </script>\
</body>\
</html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Set up the buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off at startup
  Serial.println("Buzzer pin set up.");

  // Initialize I2C communication and the MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip. Check connections.");
    while (1) {
      delay(10); // Stay here if MPU6050 is not found
    }
  }
  Serial.println("MPU6050 initialized successfully.");

  // Setup sensor ranges
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Sensor ranges set.");

  // Start WiFi in Access Point mode
  Serial.println("Starting Access Point...");
  WiFi.softAP(ap_ssid, ap_password);

  // Display IP address of the Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP Address: ");
  Serial.println(IP);

  // Start the web server
  server.on("/", handleRoot); // Serve the root webpage
  server.begin();
  Serial.println("HTTP server started");

  delay(1000);
}

void loop() {
  // Get new sensor events with the accelerometer
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Display the acceleration (in Gs)
  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(", ");
  Serial.print("Accel Y: "); Serial.print(a.acceleration.y); Serial.print(", ");
  Serial.print("Accel Z: "); Serial.print(a.acceleration.z); Serial.println(" m/s^2");

  // Define a fall threshold (you can adjust this based on your needs)
  float fallThreshold = 3.0;

  // Detect a fall based on acceleration along the Z axis
  if (abs(a.acceleration.z) < fallThreshold) {
    // Fall detected, activate buzzer and update status
    Serial.println("Fall detected! Turning buzzer ON");
    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
    fallStatus = "Fall detected!";
  } else {
    // No fall, turn off the buzzer and update status
    Serial.println("No fall detected. Turning buzzer OFF");
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
    fallStatus = "No fall detected";
  }

  // Handle web server
  server.handleClient();

  delay(2200); // Delay to prevent overwhelming serial output
}
