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

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

//This gives us a range of 60 degrees
int calibrationPos = 90;    //Set to 90 when attaching the arm
int restingPos = 90 - 30;        //We shouldn't go below 60
int pressingPos = 90 + 30;      //We shouldn't go above 120

void setup() {
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);

  AFMS.begin();
  
  leftMotor -> setSpeed(0);
  leftMotor -> run(FORWARD);
  leftMotor -> run(RELEASE);
  
  rightMotor -> setSpeed(0);
  rightMotor -> run(FORWARD);
  rightMotor -> run(RELEASE);

//  myservo.write(calibrationPos);
  myservo.write(restingPos);
  delay(2000);
}

boolean running = true;

void loop() {
  // The if statement is just so this only runs once
  if(running)
  {
//    Serial.write("This is the wind up");
//    while (!Serial.available())
//    {
//    }
//
//    Serial.readString();

    leftMotor -> setSpeed(100);
    rightMotor -> setSpeed(100);
    moveWheels(1000, FORWARD);
    moveWheels(1000, BACKWARD);
    pressButton();

    running = false;
  }
  

}


void moveWheels(int myDelayTime, int myDirection){
  // Turn the motors on
  leftMotor -> run(myDirection);
  rightMotor -> run(myDirection);

  Serial.println("forward");

  // Go for .... ms
  delay(myDelayTime);

  // Turn the motors off
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
}

void pressButton(){
  myservo.write(pressingPos);
  delay(1000);
  myservo.write(restingPos);
}

void testServoMovement(){
  // Iterates through positions from resting to pressing position
  myservo.write(calibrationPos);
  for(int i = restingPos; i <= pressingPos; i += 10){
    myservo.write(i);
    delay(500);
  }
  myservo.write(calibrationPos);
}
