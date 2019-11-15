#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>

//Adafruit_PWMServoDriver();

Servo myServo0;  // create servo object to control a servo
Servo myServo1;
Servo myServo2;
Servo myServo3;

Servo servos[4] = {myServo0, myServo1, myServo2, myServo3};

String inString;

String ordering = "31415";

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

//This gives us a range of 60 degrees
int calibrationPos = 90;    //Set to 90 when attaching the arm
int restingPos = 90 - 30;        //We shouldn't go below 60
int pressingPos = 90 + 30;      //We shouldn't go above 120

void setup() {
  // Assign pins to servos
  myServo0.attach(3);       // MAKE THESE PWM PINS!!!
  myServo1.attach(5);       // MAKE THESE PWM PINS!!!
  myServo2.attach(6);       // MAKE THESE PWM PINS!!!
  myServo3.attach(9);       // MAKE THESE PWM PINS!!!
  Serial.begin(9600);

  // Motorshield for wheels
  AFMS.begin();

  // Priming wheels
  leftMotor -> setSpeed(0);
  leftMotor -> run(FORWARD);
  leftMotor -> run(RELEASE);
  rightMotor -> setSpeed(0);
  rightMotor -> run(FORWARD);
  rightMotor -> run(RELEASE);

  // Initialize at resting position
  myServo0.write(restingPos);
  myServo1.write(restingPos);
  myServo2.write(restingPos);
  myServo3.write(restingPos);
  delay(2000);
}

boolean running = true;

void loop() {
  // The if statement is just so this only runs once
  if(running)
  {
    leftMotor -> setSpeed(100);
    rightMotor -> setSpeed(100);
    moveWheels(1000, FORWARD);

    // Pushes buttons based on 'ordering' of servos
    for (int i = 0; i < sizeof(ordering); i++){
      String charOfNum = ordering.substring(i, i+1);
      pressButton(servos[charOfNum.toInt()]);
    }

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


void pressButton(Servo servo){
  servo.write(pressingPos);
  delay(1000);
  servo.write(restingPos);
}

void testServoMovement(Servo servo){
  // Iterates through positions from resting to pressing position
  servo.write(calibrationPos);
  for(int i = restingPos; i <= pressingPos; i += 10){
    servo.write(i);
    delay(500);
  }
  servo.write(calibrationPos);
}


