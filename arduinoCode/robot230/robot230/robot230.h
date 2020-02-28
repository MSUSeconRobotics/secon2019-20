#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>
#include "src/I2Cdev.h"
#include "src/MPU6050.h"

enum State{
    start, 
    getToWall,
    dropWallClaw,
    dropWings,
    pushButtons,
    end
};

double degreesToPwm(int degree);

void calibrateButtons();

void moveWheels(int myDelayTime, int myDirection);

void pressButton(int servoNumber);

void resetState();

void startState();

void getToWallState();

void dropWallClawState();

void dropWingsState();

void pushButtonsState();

void endState();

float calibrateGyroZ();

void runMotorsWithGyro();

// Servo shield setup: called this way, it uses the default address 0x40
// Adafruit_PWMServoDriver servoShield = Adafruit_PWMServoDriver();
// You can also call it with a different address you want
Adafruit_PWMServoDriver servoShield = Adafruit_PWMServoDriver(0x41);

// Motor Shield Setup:
// AFMS: Motorshield default address is 0x60, but we have some in lab that are 0x61
Adafruit_MotorShield motorShield = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *rightMotor = motorShield.getMotor(2);
Adafruit_DCMotor *leftMotor = motorShield.getMotor(4);

// Gyro setup
MPU6050 gyro;
int zRotationTrim = 8;
double currentAngle;
unsigned long currentTime = 0, lastTimeStraight;

//target speed to go straight.
int targetSpeed = 200;

// A trim, or offset, to adjust for power difference between motors
int LR_MotorTrim = 10; 

// This will be iterated through to press buttons. Each number corresponds to a servo
String ordering = "345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456";
// String ordering = "0123456789";
// String ordering = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378";

// Our degree constants
int calibrationPos = 90; //Set to 90 when attaching the arm
int restingPosLeft = calibrationPos + 17;
int restingPosRight = calibrationPos - 17;
int pressingPosLeft = calibrationPos + 30;
int pressingPosRight = calibrationPos - 30;