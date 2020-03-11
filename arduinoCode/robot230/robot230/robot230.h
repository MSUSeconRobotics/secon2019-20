#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>
//#include "src/I2Cdev.h"
//#include "src/MPU6050.h"
#include "src/pi10000.h"

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

/**
 * piDigit - returns the 0..9 value of the dig at posn
 */
int piDigit(int posn);

void calculateServoValues();

// Servo shield setup: called this way, it uses the default address 0x40
// Adafruit_PWMServoDriver servoShield = Adafruit_PWMServoDriver();
// You can also call it with a different address you want
Adafruit_PWMServoDriver servoShield = Adafruit_PWMServoDriver(0x41);

// Motor Shield Setup:
// AFMS: Motorshield default address is 0x60, but we have some in lab that are 0x61
Adafruit_MotorShield motorShield = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *rightMotor = motorShield.getMotor(4);
Adafruit_DCMotor *leftMotor = motorShield.getMotor(2);

// Gyro setup
//MPU6050 gyro;
//int zRotationTrim = 8;
//double currentAngle;
//unsigned long currentTime = 0, lastTimeStraight;
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyZ;
float elapsedTime, currentTime, previousTime, target_angle,iValue;
float acceptableAngle = 0.1,lastStraight=0,angle = 0,offset = 0,iModifier = 0.01;
uint8_t targetSpeed = 220;
uint8_t leftMotorSpeed = targetSpeed,rightMotorSpeed = targetSpeed;

int8_t adjustmentSpeed, pValue = 5;
int motors[2]; 


// A trim, or offset, to adjust for power difference between motors
//int LR_MotorTrim = 10; 

// This will be iterated through to press buttons. Each number corresponds to a servo
// String ordering = "345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456345634563456";
// String ordering = "0123456789";
 String ordering = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378";

//                        0  1  2   3  4  5  6   7  8  9            14 15
// Baseline 10           +3 +2 +1               +1 +2 +3          +20+20
double pushingValue[] = { 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,0,0,0, 40,40};
double pushingBaseline = 15;
// Baseline 10           -4  0 -3  -1 -3  0 -4  -3 -4 -3          +7 +7
double restingValue[] = {-1, 0,0, 0,0, 5,0, 0,0,0, 0,0,-90,0, 7, 7};
double restingBaseline = 10;

int pushingDelay = 90;
int pullBackDelay = 90;

// Our degree constants
int calibrationAngle = 90; //Set to 90 when attaching the arm
double calibratingValue = degreesToPwm(calibrationAngle);
