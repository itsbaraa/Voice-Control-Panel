#include <Servo.h>
#include <SoftwareSerial.h>

const int SERVO_PIN = 13;
Servo voiceServo;
bool servoAttached = false;
int previousAngle = -1;

const byte rxPin = 2; // ESP32 TX pin 25
const byte txPin = 3; // ESP32 RX pin 27
SoftwareSerial espSerial(rxPin, txPin);

const int ANGLE_TOLERANCE = 2;

void setup() {
  Serial.begin(9600);
  Serial.println("[Arduino] Voice Servo Controller Starting...");
  
  espSerial.begin(9600);
  
  Serial.println("[Arduino] Ready to receive voice commands");
}

void loop() {
  if (espSerial.available() > 0) {
    String data = espSerial.readStringUntil('\n');
    data.trim();

    Serial.println("[Arduino] Voice Command: '" + data + "'");
    
    setServoAngle(data.toInt());
  }
}

void setServoAngle(int angle) {
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  
  if (!servoAttached) {
    voiceServo.attach(SERVO_PIN);
    voiceServo.write(90);
    previousAngle = 90;
    servoAttached = true;
    Serial.println("[Arduino] Voice servo initialized at 90°");
    delay(500);
  }
  
  if (abs(angle - previousAngle) > ANGLE_TOLERANCE) {
    voiceServo.write(angle);
    Serial.println("[Arduino] Servo moved: " + String(previousAngle) + "° → " + String(angle) + "°");
    previousAngle = angle;
    
    // Confirm movement to ESP32
    espSerial.println("MOVED");
  } else {
    Serial.println("[Arduino] Servo angle unchanged: " + String(angle) + "°");
  }
}