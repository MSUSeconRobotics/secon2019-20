/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

Servo myservo;  // create servo object to control a servo
String inString;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);

int pos = 0;    // variable to store the servo position
int restingPos = 60;
int pressingPos = 150;

void setup() {
  myservo.attach(8);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  
  leftMotor -> setSpeed(0);
  leftMotor -> run(FORWARD);
  leftMotor -> run(RELEASE);
  
  rightMotor -> setSpeed(0);
  rightMotor -> run(FORWARD);
  rightMotor -> run(RELEASE);
}

void loop() {

  leftMotor -> setSpeed(100);
  rightMotor -> setSpeed(100);
  leftMotor -> run(FORWARD);
  rightMotor -> run(FORWARD);

  delay(1000);

  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);

  pressButton();

  leftMotor -> run(BACKWARD);
  rightMotor -> run(BACKWARD);

  delay(1000);

  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);

  pressButton();

}




void pressButton(){
  myservo.write(pressingPos);
  delay(500);
  myservo.write(restingPos);
}


