#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "MPU6050.h"

int targetSpeed = 150; // Baseline motor speed
int LR_MotorOffset = 5; // Magic value we found to correct power differences between motors

MPU6050 gyro;
// Persistent values needed for the gyro's PI controller to work
// PID controller: An algorithm that uses to feedback loop with Proportional, Integral, and Derivative components to control
//    come device, often motors
// Our algorithm only used P and I components with no Derivative, thus a PI controller
//  Ref: https://spin.atomicobject.com/2016/06/28/intro-pid-control/
double currentAngle;
double currentTime;
double lastTimeStraight; // Used with integral component
int ZRotationOffset = 8; // Magic valueswe found to be a constant offset

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
  delay(1000); // Unsure of the need for this value
  
  // Initial setup for PI controller
  currentTime = millis()/1000;
  lastTimeStraight = currentTime;
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

  // Since the gyro is positive counterclockwise, we must speed up the left wheel to counter act
  int leftSpeed = targetSpeed + speedDelta + LR_MotorOffset;
  int rightSpeed = targetSpeed - speedDelta - LR_MotorOffset;

  // Limit cap and min
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
  double pastTime = currentTime;

  double angleVelocity = gyro.getRotationZ() + ZRotationOffset;
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