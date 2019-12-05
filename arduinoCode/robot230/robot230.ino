#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Motor Shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

String userInput = "";
String ordering = "3456";

int calibrationPos = 90;    //Set to 90 when attaching the arm
int restingPos = 90-15;
int pressingPosLeft = restingPos+15;
int pressingPosRight = restingPos-15;

int restingValue = 725/180*restingPos + 275;
int pressingValueLeft = 725/180*pressingPosLeft + 275;
int pressingValueRight = 725/180*pressingPosRight + 275;
int calibratingValue = 725/180*calibrationPos + 275;

boolean running = true;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  Serial.println("Input values 1-4095");

  pwm.begin();
  pwm.setPWMFreq(100);   

  // UNCOMMENT to put arm at middle position
//  for(int i=0; i<=10; i++){
//    pwm.setPWM(i,0,calibratingValue);
//    delay(1000);
//  }
  
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
//    moveWheels(1000, FORWARD);
//    moveWheels(1000, BACKWARD);
    
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
  if (servoNumber <= 4){
    pwm.setPWM(servoNumber,0,pressingValueLeft);
    delay(250);
    pwm.setPWM(servoNumber,0,restingValue);
  } else {
    pwm.setPWM(servoNumber,0,pressingValueRight);
    delay(250);
    pwm.setPWM(servoNumber,0,restingValue);
  }
}
