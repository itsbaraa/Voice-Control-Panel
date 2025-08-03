#include <WiFi.h>
#include <HTTPClient.h>

#define ARDUINO_SERIAL Serial2
const int ARDUINO_RX_PIN = 27;
const int ARDUINO_TX_PIN = 25;

const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";
const char* serverIP = "your_server_ip";

String directionUrl = String("http://") + serverIP + "/direction.php";

const int LED_PIN = 2;

unsigned long lastCheckTime = 0;
const long checkInterval = 1000;
String lastDirection = "";
int lastAngle = -1;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  ARDUINO_SERIAL.begin(9600, SERIAL_8N1, ARDUINO_RX_PIN, ARDUINO_TX_PIN);
  
  Serial.println("\n[ESP32] Voice Servo Controller Starting...");
  
  setupWiFi();
}

void setupWiFi() {
  Serial.println("[WiFi] Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected successfully!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_PIN, HIGH); // LED on when connected
  } else {
    Serial.println("\n[WiFi] Connection failed!");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("[WiFi] Reconnecting...");
    WiFi.begin(ssid, password);
    delay(5000);
    return;
  }
  
  digitalWrite(LED_PIN, HIGH);
  
  if (millis() - lastCheckTime > checkInterval) {
    lastCheckTime = millis();
    checkVoiceCommand();
  }
  
  if (ARDUINO_SERIAL.available() > 0) {
    String message = ARDUINO_SERIAL.readStringUntil('\n');
    message.trim();
    if (message == "MOVED") {
      Serial.println("[Arduino] Servo movement confirmed");
    }
  }
  
  delay(50);
}

void checkVoiceCommand() {
  HTTPClient http;
  http.setTimeout(5000);
  http.begin(directionUrl);
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    if (response.indexOf("\"success\":true") > -1) {
      int directionStart = response.indexOf("\"direction\":\"") + 13;
      int directionEnd = response.indexOf("\"", directionStart);
      
      int angleStart = response.indexOf("\"angle\":") + 8;
      int angleEnd = response.indexOf(",", angleStart);
      if (angleEnd == -1) angleEnd = response.indexOf("}", angleStart);
      
      if (directionStart > 12 && directionEnd > directionStart && 
          angleStart > 7 && angleEnd > angleStart) {
        
        String currentDirection = response.substring(directionStart, directionEnd);
        int currentAngle = response.substring(angleStart, angleEnd).toInt();
        
        // Only send command if direction changed
        if (currentDirection != lastDirection && currentDirection != "none") {
          Serial.println("[Voice] Command: " + currentDirection + " (" + currentAngle + "°)");
          ARDUINO_SERIAL.println(currentAngle);
          
          lastDirection = currentDirection;
          lastAngle = currentAngle;
        }
      }
    }
  } else {
    Serial.println("[HTTP] Failed to get voice command: " + String(httpCode));
  }
  
  http.end();
}