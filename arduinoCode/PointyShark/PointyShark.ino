#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Motor Shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);

String userInput = "";
String ordering = "010101011";

int calibrationPos = 90;    //Set to 90 when attaching the arm
int restingPos = 90 - 45;        //We shouldn't go below 60
int pressingPos = 90 + 45;      //We shouldn't go above 120

int restingValue = 725/180*restingPos + 275;
int pressingValue = 725/180*pressingPos + 275;
int calibratingValue = 725/180*calibrationPos + 275;

boolean running = true;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  Serial.println("Input values 1-4095");

  pwm.begin();
  pwm.setPWMFreq(100);   

  // UNCOMMENT to put arm at middle position
//  pwm.setPWM(0,0,calibratingValue);
//  pwm.setPWM(1,0,calibratingValue);
//  delay(10000); 

  // Motor setup
  AFMS.begin();
  
  leftMotor -> setSpeed(0);
  leftMotor -> run(FORWARD);
  leftMotor -> run(RELEASE);
  
  rightMotor -> setSpeed(0);
  rightMotor -> run(FORWARD);
  rightMotor -> run(RELEASE);
}

void loop() {
  long pwmValue = 1;

  if(running)
  {
    // Pushes buttons based on 'ordering' of servos
    leftMotor -> setSpeed(100);
    rightMotor -> setSpeed(100);

    moveToPosition();
    
    for (int i = 0; i < sizeof(ordering); i++){
      String charOfNum = ordering.substring(i, i+1);
      pressButton(charOfNum.toInt());
      delay(1000);
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

void pressButton(int servoNumber){
  pwm.setPWM(servoNumber,0,pressingValue);
  delay(250);
  pwm.setPWM(servoNumber,0,restingValue);
}

void moveToPosition(){
  moveWheels(1000, FORWARD);
}
