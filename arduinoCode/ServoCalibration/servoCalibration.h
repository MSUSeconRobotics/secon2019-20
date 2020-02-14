#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

double degreesToPwm(int degree);

void calibrateButtons();

// Servo shield setup: called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

String ordering = "0110";
// String ordering = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378";

// Our degree constants
int calibrationPos = 90; //Set to 90 when attaching the arm