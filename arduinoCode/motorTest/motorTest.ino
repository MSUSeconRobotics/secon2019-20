#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "MPU6050.h"

int targetSpeed = 150;

MPU6050 gyro;
double currentAngle;
double currentTime = 0.0;
double lastTimeStraight;

// Motor Shield
Adafruit_MotorShield motorShield = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = motorShield.getMotor(4);
Adafruit_DCMotor *leftMotor = motorShield.getMotor(2);

void setup() {
  Serial.begin(115200);
  // Motor setup
  motorShield.begin();
  
  leftMotor -> setSpeed(targetSpeed);
  leftMotor -> run(RELEASE);
  
  rightMotor -> setSpeed(targetSpeed);
  rightMotor -> run(RELEASE);

  gyro.initialize();
  currentAngle = 0;
  delay(1000);
  updateCurrentAngle();
  
  leftMotor -> run(FORWARD);
  rightMotor -> run(FORWARD);
}

void loop() 
{
  updateCurrentAngle();
  double proportional = currentAngle*3; // Proportional magnifier
  double integral = getIntegralComponent()*15; // Integral magnifier

  int speedDelta = proportional + integral;

  int leftSpeed = targetSpeed + speedDelta; // Evidently our left motor is weaker
  int rightSpeed = targetSpeed - speedDelta;
  if (leftSpeed > 255)
    leftSpeed = 255;
  else if (leftSpeed < 50)
    leftSpeed = 50;

  if (rightSpeed > 255)
    rightSpeed = 255;
  else if (rightSpeed < 50)
    rightSpeed = 50;

  leftMotor -> setSpeed(leftSpeed);
  rightMotor -> setSpeed(rightSpeed);
}

void updateCurrentAngle()
{
  // Create baseline on first call
  if (currentTime == 0)
  {
    currentTime = millis()/1000;
    lastTimeStraight = currentTime;
  }

  double pastTime = currentTime;

  double angleVelocity = gyro.getRotationZ() + 8; // This value is what we found to be a constant offset
  angleVelocity = angleVelocity / 131.0; // This value is defined by the gryo's scale and sensitivity

  currentTime = millis()/1000.0;
  double deltaTime = currentTime - pastTime;
  
  double deltaAngle = angleVelocity * deltaTime;

  // If delta is greater than and opposite, it will pass through 0
  if (currentAngle*deltaAngle < 0 && abs(deltaAngle) > abs(currentAngle)) 
  {
    lastTimeStraight = currentTime;
  }
  currentAngle = currentAngle + deltaAngle;
}

double getIntegralComponent()
{
  // Time since it was straight
  double deltaTime = (millis()/1000) - lastTimeStraight;
  Serial.println(deltaTime);
  return currentAngle * abs(currentAngle) * deltaTime;
}