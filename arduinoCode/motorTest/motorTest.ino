#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 gyro;

int gyroCorrection();

int targetSpeed = 125;
int currentTime = 0;
float currentDirectionDegrees = 0;

// Motor Shield
Adafruit_MotorShield motorShield = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = motorShield.getMotor(4);
Adafruit_DCMotor *leftMotor = motorShield.getMotor(1);

void setup() {
//   Serial.begin(9600);
//   Serial.println("Motor test");
  
  currentTime = millis();
  gyro.initialize(); // Must be called after afms.begin
  delay(250);

  // Motor setup
  motorShield.begin();
  
  leftMotor -> setSpeed(targetSpeed);
  leftMotor -> run(FORWARD);
  
  rightMotor -> setSpeed(targetSpeed);
  rightMotor -> run(FORWARD);
//   delay(20);
}

void loop() {
    // int angleCorrection = gyroCorrection();
    // // Serial.print("Motor delta: ");
    // // Serial.println(angleCorrection);
    
    // int newSpeed = (angleCorrection * 5) + targetSpeed;
    // if (newSpeed > 255)
    //     newSpeed = 255;
    // if (newSpeed < 0)
    //     newSpeed = 0;

    // // delay(20);
    // leftMotor -> setSpeed(newSpeed);
    // leftMotor -> run(FORWARD);
    // delay(20);
}

int gyroCorrection()
{
    // Getting time elapsed in seconds
    int previousTime = currentTime;
    currentTime = millis();                            // Current time actual time read
    float elapsedTime = (currentTime - previousTime) / 1000.0; // In seconds

    float angularVelocity = gyro.getRotationZ()/131.0; // Found this scaling in a spec sheet
    float angleDelta = angularVelocity * elapsedTime; // Convert Deg/s to Deg
    currentDirectionDegrees = currentDirectionDegrees + angleDelta;

    // Serial.print("Current angle: ");
    // Serial.println(currentDirectionDegrees);

    int speedDifferential = currentDirectionDegrees; // To be tested and fixed
    return speedDifferential;
}