#include "robot230Minimum.h"

State state = start;

// Converte degrees to PWM for servo sheild
double restingValueLeft = degreesToPwm(restingPosLeft);
double restingValueRight = degreesToPwm(restingPosRight);
double pressingValueLeft = degreesToPwm(pressingPosLeft);
double pressingValueRight = degreesToPwm(pressingPosRight);
double calibratingValue = degreesToPwm(calibrationPos);

void setup()
{
    Serial.begin(115200);
    Serial.println("Robot230 Minimum");

    // Servo setup
    pwm.begin();
    pwm.setPWMFreq(100);

    // Motor setup
    AFMS.begin();

    leftMotor->setSpeed(0);
    leftMotor->run(FORWARD);
    leftMotor->run(RELEASE);

    rightMotor->setSpeed(0);
    rightMotor->run(FORWARD);
    rightMotor->run(RELEASE);

    leftMotor->setSpeed(targetSpeed);
    rightMotor->setSpeed(targetSpeed);

    // Limit switch setup
    pinMode(12, INPUT_PULLUP);
}

int positionInPi = 0;

void loop()
{
    // Serial.println("State: ");
    // Serial.println(state);

    switch (state)
    {
    case start:
        startState();
        break;

    case dropWings:
        dropWingsState();
        // TODO: Change calibrate buttons to put buttons in pre press state
        calibrateButtons();
        break;

    case getToWall:
        getToWallState();
        break;

    case pushButtons:
        pushButtonsState();
        break;

    case end:
        endState();
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
    if (servoNumber == 0 || servoNumber == 1 || servoNumber == 3 || servoNumber == 4 || servoNumber == 7 || servoNumber == 15)
        direction = 1;
    else
        direction = 0;

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

void startState()
{
    if (digitalRead(12))
    {
        while (digitalRead(12))
        {
            Serial.println("Waiting for RELEASE");
        }

        state = dropWings;
    }
    return;
}

void getToWallState()
{
    // leftMotor->run(FORWARD);
    // rightMotor->run(FORWARD);

    while (!digitalRead(12))
    {
        Serial.println("Getting to the wall");

        leftMotor -> run(FORWARD);
        rightMotor -> run(FORWARD);
    }
        
    state = pushButtons;
    return;
}
                                             
void dropWingsState()
{
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);

    pwm.setPWM(14, 0, degreesToPwm(120));
    pwm.setPWM(15, 0, degreesToPwm(50));
    delay(500);

    pwm.setPWM(14, 0, calibratingValue);
    pwm.setPWM(15, 0, calibratingValue);

    state = getToWall;
    return;
}

void pushButtonsState()
{
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);

    String charOfOrdering = ordering.substring(positionInPi, positionInPi + 1);
    pressButton(charOfOrdering.toInt());

    // TODO: change to elapsed time?
    delay(1000);

    if (positionInPi >= ordering.length())
        state = end;
    else if (!digitalRead(12))
        state = getToWall;

    positionInPi++;
    return;
}

void endState()
{
    for (int i = 0; i < 15; i++)
    {

        if (i == 2)
            pwm.setPWM(i, 0, pressingValueRight);
        else if (i == 7)
            pwm.setPWM(i, 0, pressingValueLeft);
        else if (1 > 12)
            pwm.setPWM(i, 0, calibratingValue);
        // TODO: we aren't using servos 10 - 12, should we skip them in this loop?
    }

    state = -1;
    return;
}