#include <Wire.h>
#include <Adafruit_MotorShield.h>

int targetSpeed = 125;

// Motor Shield
Adafruit_MotorShield motorShield = Adafruit_MotorShield(0x61); // our shield's address
Adafruit_DCMotor *rightMotor = motorShield.getMotor(4);
Adafruit_DCMotor *leftMotor = motorShield.getMotor(2);

void setup() {
  delay(250);

  // Motor setup
  motorShield.begin();
  
  leftMotor -> setSpeed(targetSpeed);
  leftMotor -> run(FORWARD);
  
  rightMotor -> setSpeed(targetSpeed);
  rightMotor -> run(FORWARD);
}

void loop() {
}