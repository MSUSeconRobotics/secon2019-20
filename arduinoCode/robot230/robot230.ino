#include "robot230.h"

State state = start;

// Converte degrees to PWM for servo sheild
double restingValue = degreesToPwm(restingPos);
double pressingValueLeft = degreesToPwm(pressingPosLeft);
double pressingValueRight = degreesToPwm(pressingPosRight);
double calibratingValue = degreesToPwm(calibrationPos);

void setup()
{
    Serial.begin(9600);
    Serial.println("16 channel PWM test!");
    Serial.println("Input values 1-4095");

    // Servo setup
    pwm.begin();
    pwm.setPWMFreq(100);

    // calibrateButtons();

    // Motor setup
    AFMS.begin();

    leftMotor->setSpeed(0);
    leftMotor->run(FORWARD);
    leftMotor->run(RELEASE);

    rightMotor->setSpeed(0);
    rightMotor->run(FORWARD);
    rightMotor->run(RELEASE);

    leftMotor->setSpeed(250);
    rightMotor->setSpeed(250);

    // Limit switch setup
    pinMode(12, INPUT_PULLUP);
}

boolean reset = false;
int i = 0;

void loop()
{
    Serial.println("State: ");
    Serial.println(state);

    switch (state)
    {
    case start:
        if (digitalRead(12))
        {
            while (digitalRead(12))
                Serial.println("Waiting for RELEASE");

            state = getToWall;
        }
            
        break;

    case dropWings:
        leftMotor->run(RELEASE);
        rightMotor->run(RELEASE);

        state = getToWall;
        break;

    case getToWall:
        leftMotor->run(FORWARD);
        rightMotor->run(FORWARD);

        while (!digitalRead(12))
            Serial.println("Getting to the wall");

        state = pushButtons;
        break;
    
    case pushButtons:
        leftMotor->run(RELEASE);
        rightMotor->run(RELEASE);

        String charOfOrdering = ordering.substring(i, i + 1);
        pressButton(charOfOrdering.toInt());
        
        // TODO: change to elapsed time?
        delay(1000);

        if (i >= ordering.length())
            state = end;
        else if (!digitalRead(12))
            state = getToWall;
        
        i++;

        break;

    case end:
        if (!reset)
            resetState();
        break;

    default:
        break;
    }
}

void calibrateButtons()
{
    for (int i = 0; i < 10; i++)
        pwm.setPWM(i, 0, calibratingValue);
}

void moveWheels(int myDelayTime, int myDirection)
{
    // Turn the motors on
    leftMotor->run(myDirection);
    rightMotor->run(myDirection);

    Serial.println("forward");

    // TODO: changed to elapsed time
    // Go for .... ms
    delay(myDelayTime);

    // Turn the motors off
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
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

void resetState(){
    for (int i = 0; i <= 15; i++)
    {
        if (i != 10 || i != 11 || i != 12)
        {
            pwm.setPWM(servoNumber, 0, restingValue);
        }
    }
    reset = true;
}