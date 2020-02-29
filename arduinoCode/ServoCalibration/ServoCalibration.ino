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
bool isLeftServo(int servoNumber);

double calibratingValue;

double restingValueLeft;
// double pressingValueLeft;

double restingValueRight;
// double pressingValueRight;

int pushingDelay = 250;
int pullBackDelay = 500;

void setup() {
    int calibrationAngle = 90; //Set to 90 when attaching the arm
    // int pushingDelta = 36;
    int restingDelta = 17;

    int* pushingDelta = [15,15,15, 15,15,15,15, 15,15,15, 0,0,0,0, 30,30];
    double* pushingValue = [90,90,90, 90,90,90,90, 90,90,90, 90,90,90,90, 90,90];

    calibratingValue = degreesToPwm(calibrationAngle);

    restingValueLeft = degreesToPwm(calibrationAngle + restingDelta);
    // pressingValueLeft = degreesToPwm(calibrationAngle + pushingDelta);

    restingValueRight = degreesToPwm(calibrationAngle - restingDelta);
    // pressingValueRight = degreesToPwm(calibrationAngle - pushingDelta);

  // Servo setup
    pwm.begin();
    pwm.setPWMFreq(100);

    Serial.begin(115200);
    Serial.println("yooo");

    for (int i = 0; i <= 15; i++)
    {       
        // TODO: change to elapsed time
        if (isLeftServo(i)) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
        {
            pwm.setPWM(i, 0, restingValueLeft);
            pushingValue[i] = degreesToPwm(calibrationAngle + pushingDelta[i]);
        }
        else
        {
            pwm.setPWM(i, 0, restingValueRight);
            pushingValue[i] = degreesToPwm(calibrationAngle - pushingDelta[i]);
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
            delay(pullBackDelay);
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
        for (int i = 0; i <= 15; i++)
        {   
            pwm.setPWM(i, 0, calibratingValue);
        }
    }
    
}

void pressButton(int servoNumber)
{    
    // TODO: change to elapsed time
    if (isLeftServo(servoNumber)) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
    {
        pwm.setPWM(servoNumber, 0, pushingValue[servoNumber]);
        delay(pushingDelay);
        pwm.setPWM(servoNumber, 0, restingValueLeft);
    }
    else
    {
        pwm.setPWM(servoNumber, 0, pushingValue[servoNumber]);
        delay(pushingDelay);
        pwm.setPWM(servoNumber, 0, restingValueRight);
    }
}

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}

bool isLeftServo(int servoNumber)
{
    return servoNumber == 0 || servoNumber == 1 || servoNumber == 3 || servoNumber == 4 || servoNumber == 7 || servoNumber == 15;
}