#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>

enum State{
    start, 
    getToWall,
    dropWings,
    pushButtons,
    end
};

double degreesToPwm(int degree);

void calibrateButtons();

void moveWheels(int myDelayTime, int myDirection);

void pressButton(int servoNumber);

// Servo shield setup: called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// You can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Motor Shield Setup:
// AFMS: Motorshield default address is 0x60, but we have some in lab that are 0x61
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

// This will be iterated through to press buttons. Each number corresponds to a servo
String ordering = "0110";
// String ordering = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378";

// Our degree constants
int calibrationPos = 90; //Set to 90 when attaching the arm
int deltaDegrees = 30;
int restingPos = 80;
int pressingPosLeft = restingPos + deltaDegrees;
int pressingPosRight = restingPos - deltaDegrees;