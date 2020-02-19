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

    // Gyro setup
    gyro.initialize();

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
        startState();
        break;

    case dropWings:
        dropWingsState();
        break;

    case getToWall:
        getToWallState();
        break;

    case dropWallClaw:
        // TODO: work out the configuration details of the wall grabber
        // dropWallClawState();
        state = pushButtons;
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
    {
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

void resetState()
{
    for (int i = 0; i < 9; i++)
    {
        pwm.setPWM(i, 0, restingValue);
    }

    pwm.setPWM(13, 0, calibratingValue);
    pwm.setPWM(14, 0, calibratingValue);
    pwm.setPWM(15, 0, calibratingValue);

    reset = true;
}

void startState()
{
    if (digitalRead(12))
    {
        while (digitalRead(12))
        {
            Serial.println("Waiting for RELEASE");
        }

        // Get initial gyro values
        zRotationCalibration = calibrateGyroZ();
        Serial.println("gyro vals");
        Serial.println(zRotationCalibration);

        state = dropWings;
    }
    return;
}

void getToWallState()
{
    // leftMotor->run(FORWARD);
    // rightMotor->run(FORWARD);
    runMotorsWithGyro();

    // while (!digitalRead(12))
    //     Serial.println("Getting to the wall");

    // state = dropWallClaw;
    return;
}

void dropWallClawState()
{
    pwm.setPWM(13, 0, degreesToPwm(0));
    delay(2000);
    state = pushButtons;
    return;
}

void dropWingsState()
{
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);

    pwm.setPWM(14, 0, pressingValueLeft);
    pwm.setPWM(15, 0, pressingValueRight);
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

    String charOfOrdering = ordering.substring(i, i + 1);
    pressButton(charOfOrdering.toInt());

    // TODO: change to elapsed time?
    delay(1000);

    if (i >= ordering.length())
        state = end;
    else if (!digitalRead(12))
        state = getToWall;

    i++;
    return;
}

void endState()
{
    if (!reset)
        resetState();

    return;
}

float calibrateGyroZ()
{
    float GyZ_avg = 0;
    for (int x = 0; x < 1000; x++)
    {
        GyZ_avg = gyro.getRotationZ() + GyZ_avg;
    }
    GyZ_avg = GyZ_avg / 1000.0;
    return GyZ_avg;
}

void runMotorsWithGyro()
{
    previousTime = currentTime;                        // Previous time is stored before the actual time read
    currentTime = millis();                            // Current time actual time read
    elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

    zRotation = gyro.getRotationZ();

    //subtract offset
    zRotation = zRotation - zRotationCalibration;

    //convert from rad/s to rad
    angle = angle + zRotation * elapsedTime;
    Serial.print("angle = ");
    Serial.println(angle);

    //turned to the left, we'll increase power to left wheel to compensate (and drop power to right)
    if (angle > 0)
    {
        //change constant based on what power step size works for your motor.
        copSpeed = 11 * angle;

        //combine target speed with the speed to compensate for angle
        totalSpeed = copSpeed + targetSpeed;
        
        //make sure speed is 255 and less.
        if (totalSpeed > 255)
        {
            totalSpeed = 255;
        }
        leftMotor->setSpeed(totalSpeed);
        leftMotor->run(FORWARD);

        totalSpeed = targetSpeed - copSpeed;
        if (totalSpeed < 0)
        {
            totalSpeed = 0;
        }
        rightMotor->setSpeed(totalSpeed);
        rightMotor->run(FORWARD);
    }

    //vice versa
    else
    {
        copSpeed = -11 * angle;
        totalSpeed = copSpeed + targetSpeed;
        if (totalSpeed > 255)
        {
            totalSpeed = 255;
        }
        rightMotor->setSpeed(totalSpeed);
        rightMotor->run(FORWARD);

        totalSpeed = targetSpeed - copSpeed;
        if (totalSpeed < 0)
        {
            totalSpeed = 0;
        }
        leftMotor->setSpeed(totalSpeed);
        leftMotor->run(FORWARD);
    }
}