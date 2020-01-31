// This script is a demonstration of how to use the MPU6050 library.
// Many aspects of this script are not required to run the MPU6050, and this 
// script is not exhaustive of its abilities. Please reference the library's 
// repo at: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

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

  /* Methods of interest
    All the units below are currently unknown

    // Motion
    bool  getIntMotionStatus() : Maybe it has motion flag? Seems of cool
    bool  getXNegMotionDetected() : Integral of acceleration?
    ... YNeg, ZNeg, XPos, YPos, ZPos ...
    bool  getZeroMotionDetected()

    // Multi-sensor accessors
    void  getMotion9(ax, ay, az, gx, gy, gz, mx, my, mz) : Accel(x,y,z) Gyro/Rotation(x,y,z) M?(x,y,z) Motion?
    void  getMotion6(ax, ay, az, gx, gy, gz) : Get all 6 main values

    void  getAcceleration(x, y, z) : Linear accelerations
    void  getRotation(x, y, z) : Spinning rotation

    // Individual sensor accessors
    int16 getAccelerationX()
    int16 getAccelerationY()
    int16 getAccelerationZ()

    int16 getRotationX()
    int16 getRotationY()
    int16 getRotationZ()

    int16 getTemperature()

    // Resets?
    void resetGyroscopePath() : What is a path?
    void resetAccelerationPath()
    void resetTemperaturePath()
    void resetSensors() : Possibly useful?

    // "PWR_MGMT_1 register"
    void reset()
    void setSleepEnabled(bool) : Wake up the device?
    void setWakeCycleEnabled(bool) : Wake up device?
    void setTempSensorEnabled(bool) : I guess it can be toggled

    // Calibration
    void CalibrateGyro(loops = 15) : "Fine tune after setting offsets with less Loops."
    void CalibrateAccel(loops = 15) : "Fine tune after setting offsets with less Loops."
    void PrintActiveOffsets() : See the calibration results
  */

  Serial.print("Rotation(x,y,z):\t");
  Serial.print(xRotation); Serial.print("\t");
  Serial.print(yRotation); Serial.print("\t");
  Serial.println(zRotation);
}