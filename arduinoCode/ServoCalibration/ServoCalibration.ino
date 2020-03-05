/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);


double degreesToPwm(int degree);
bool isLeftServo(int servoNumber);

double calibratingValue;
double restingValueLeft;
double restingValueRight;

//                        0  1  2   3  4  5  6   7  8  9            14 15
// Baseline 10           +3 +2 +1               +1 +2 +3
double pushingValue[] = {13,12,11, 10,10,10,10, 11,12,13, 0,0,0,0, 30,30};
// Baseline 10           -4  0 -3  -1 -3  0 -4  -3 -4 -3
double restingValue[] = { 6,10, 7,  0, 7,10, 6,  7, 6, 7, 0,0,0,0, 17,17};

int pushingDelay = 70;
int pullBackDelay = 70;

void setup() {
    int calibrationAngle = 90; //Set to 90 when attaching the arm

    calibratingValue = degreesToPwm(calibrationAngle);

    // Servo setup
    pwm.begin();
    pwm.setPWMFreq(100);

    Serial.begin(115200);
    Serial.println("yooo");

    for (int i = 0; i <= 15; i++)
    {
        // pwm.setPWM(i, 0, calibratingValue);
        // delay(100);
        // TODO: change to elapsed time
        if (isLeftServo(i)) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
        {
            // Note that order matters here
            pushingValue[i] = degreesToPwm(calibrationAngle + restingValue[i] + pushingValue[i]);
            restingValue[i] = degreesToPwm(calibrationAngle + restingValue[i]);
            pwm.setPWM(i, 0, restingValue[i]);
        }
        else
        {
            // Note that order matters here
            pushingValue[i] = degreesToPwm(calibrationAngle - restingValue[i] - pushingValue[i]);
            restingValue[i] = degreesToPwm(calibrationAngle - restingValue[i]);
            pwm.setPWM(i, 0, restingValue[i]);

        }
    }
    delay(500);
}

boolean running = true;

void loop()
{
    // delay(1);
    // return;
    // running = false;
    if (running)
    {
        for (int i = 0; i <= 9; i++)
        {   
            pressButton(i);
            delay(pullBackDelay);
        }
        for (int i = 14; i <= 15; i++)
        {
            pressButton(i);
            delay(pullBackDelay); 
        }
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
    // if (isLeftServo(servoNumber)) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
    // {
        pwm.setPWM(servoNumber, 0, pushingValue[servoNumber]);
        delay(pushingDelay);
        pwm.setPWM(servoNumber, 0, restingValue[servoNumber]);
    // }
    // else
    // {
    //     pwm.setPWM(servoNumber, 0, pushingValue[servoNumber]);
    //     delay(pushingDelay);
    //     pwm.setPWM(servoNumber, 0, restingValueRight);
    // }
}

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}

bool isLeftServo(int servoNumber)
{
    return servoNumber == 0 || servoNumber == 1 || servoNumber == 3 || servoNumber == 4 || servoNumber == 7 || servoNumber == 14;
}