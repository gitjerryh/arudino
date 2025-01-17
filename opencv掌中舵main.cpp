[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
lib_deps =
    madhephaestus/ESP32Servo @ ^0.13.0

build_flags = 
    -DCORE_DEBUG_LEVEL=5


#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo;
const int servoPin = 3;  // GPIO3

void setup() {
  Serial.begin(115200);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  
  myservo.attach(servoPin, 500, 2400);
  
  // 测试舵机
  myservo.write(90);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  
  Serial.println("Servo initialized");
}

void loop() {
  if(Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int angle = data.toInt();
    
    angle = constrain(angle, 0, 180);
    myservo.write(angle);
    
    // 发送确认
    Serial.print("Moved to angle: ");
    Serial.println(angle);
  }
}
