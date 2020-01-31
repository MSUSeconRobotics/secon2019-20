#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>

enum State{
    start, 
    getToWall,
    dropWings,
    adjustPosition,
    pushButtons,
    end
};

double degreesToPwm(int degree);

void calibrateButtons();

void moveWheels(int myDelayTime, int myDirection);

void pressButton(int servoNumber);