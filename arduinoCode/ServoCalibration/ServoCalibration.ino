/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

double degreesToPwm(int degree);

String inString;
// twelve servo objects can be created on most boards

int pos = 0;
int calibrationPos = 90; //Set to 90 when attaching the arm
int deltaDegrees = 30;
int restingPos = 80;
int pressingPosLeft = restingPos + deltaDegrees;
int pressingPosRight = restingPos - deltaDegrees;

double restingValue = degreesToPwm(restingPos);
double pressingValueLeft = degreesToPwm(pressingPosLeft);
double pressingValueRight = degreesToPwm(pressingPosRight);
double calibratingValue = degreesToPwm(calibrationPos);

void setup() {
  // Servo setup
  pwm.begin();
  pwm.setPWMFreq(100);

  Serial.begin(9600);
  Serial.println("yooo");
}

boolean running = true;

void loop()
{
    if (running)
    {
        for (int i = 0; i <= 9; i++)
        {   
            pressButton(i);
            delay(500);
            pwm.setPWM(i, 0, calibrationPos);
        }

        for (int i = 13; i <= 15; i++)
        {   
            pressButton(i);
            delay(500);
            pwm.setPWM(i, 0, calibrationPos);
        }
        running = false;
    }
}

void pressButton(int servoNumber)
{
    int direction = 0;
    if (servoNumber == 0 || servoNumber == 1 || servoNumber == 3 || servoNumber == 4 || servoNumber == 7 || servoNumber == 15){
      direction = 1;
    }
    else
    {
      direction = 0;
    }
    
    // TODO: change to elapsed time
    if (direction) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
    {
        pwm.setPWM(servoNumber, 0, pressingValueLeft);
        delay(250);
        pwm.setPWM(servoNumber, 0, restingValue);
    }
    else
    {
        pwm.setPWM(servoNumber, 0, pressingValueRight);
        delay(250);
        pwm.setPWM(servoNumber, 0, restingValue);
    }
}

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}