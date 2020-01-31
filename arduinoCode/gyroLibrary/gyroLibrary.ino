#include <Arduino.h>

// MPU6050 is dependent upon Wire and I2Cdev
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 gyro;
// These variables allow us to retrieve the gyro's values
int16_t xAccel, yAccel, zAccel;
int16_t xRotation, yRotation, zRotation;

void setup() {
  Wire.begin();

  Serial.begin(115200);
  Serial.println("This is a gyro test");
  gyro.initialize(); // Unsure what this does? May not be necessary

  // use the code below to change accel/gyro offset values
  Serial.println("Updating internal sensor offsets...");
  
  // Example of how to check the current offset values which are used for calibration
  Serial.print(gyro.getXAccelOffset()); Serial.print("\t");
  Serial.print(gyro.getYAccelOffset()); Serial.print("\t");
  Serial.print(gyro.getZAccelOffset()); Serial.print("\t");
  Serial.print(gyro.getXGyroOffset()); Serial.print("\t");
  Serial.print(gyro.getYGyroOffset()); Serial.print("\t");
  Serial.print(gyro.getZGyroOffset()); Serial.print("\t");
  Serial.print("\n");

  // Example of how to set desired offset values, perhaps with known calibration
  gyro.setXGyroOffset(220);
  gyro.setYGyroOffset(76);
  gyro.setZGyroOffset(-85);
}

void loop() {
  // Example of one way to pull rotation data from MPU6050
  gyro.getRotation(&xRotation, &yRotation, &zRotation);

  Serial.print("Rotation(x,y,z):\t");
  Serial.print(xRotation); Serial.print("\t");
  Serial.print(yRotation); Serial.print("\t");
  Serial.println(zRotation);
}