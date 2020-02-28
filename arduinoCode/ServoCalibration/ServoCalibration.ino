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

int calibrationAngle = 90; //Set to 90 when attaching the arm
int pushingDelta = 36;
int restingDelta = 17;

double calibratingValue = degreesToPwm(calibrationAngle);

double restingValueLeft = degreesToPwm(calibrationAngle + restingDelta);
double pressingValueLeft = degreesToPwm(calibrationAngle + pushingDelta);

double restingValueRight = degreesToPwm(calibrationAngle - restingDelta);
double pressingValueRight = degreesToPwm(calibrationAngle - pushingDelta);

void setup() {
  // Servo setup
    pwm.begin();
    pwm.setPWMFreq(100);

    Serial.begin(115200);
    Serial.println("yooo");

    for (int i = 0; i <= 14; i++)
    {   
        int direction = 0;
        if (i == 0 || i == 1 || i == 3 || i == 4 || i == 7 || i == 15){
            direction = 1;
        }
        else
        {
            direction = 0;
        }
        
        // TODO: change to elapsed time
        if (direction) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
        {
            pwm.setPWM(i, 0, restingValueLeft);
        }
        else
        {
            pwm.setPWM(i, 0, restingValueRight);
        }
    }
    delay(500);
}

boolean running = true;

void loop()
{
    if (running)
    {
        // pressButton(1);
        // delay(500);
        for (int i = 0; i <= 9; i++)
        {   
            pressButton(i);
            delay(500);
        }

        // for (int i = 13; i <= 15; i++)
        // {   
        //     pressButton(i);
        //     delay(500);
        //     // pwm.setPWM(i, 0, calibrationAngle);
        // }
        running = false;
    }
    else
    {
        delay(500);
        for (int i = 0; i <= 14; i++)
        {   
            pwm.setPWM(i, 0, calibratingValue);
        }
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
        pwm.setPWM(servoNumber, 0, restingValueLeft);
    }
    else
    {
        pwm.setPWM(servoNumber, 0, pressingValueRight);
        delay(250);
        pwm.setPWM(servoNumber, 0, restingValueRight);
    }
}

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}