#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

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
}

void loop() {
  long pwmValue = 1;

  if(running)
  {
    // Pushes buttons based on 'ordering' of servos
    for (int i = 0; i < sizeof(ordering); i++){
      String charOfNum = ordering.substring(i, i+1);
      pressButton(charOfNum.toInt());
      delay(1000);
    }

    running = false;
  }
}



void pressButton(int servoNumber){
  pwm.setPWM(servoNumber,0,pressingValue);
  delay(250);
  pwm.setPWM(servoNumber,0,restingValue);
}

