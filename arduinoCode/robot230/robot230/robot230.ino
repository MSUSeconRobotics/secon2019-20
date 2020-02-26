#include "robot230.h"

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
    Serial.println("16 channel PWM test!");
    Serial.println("Input values 1-4095");

    // Servo setup
    servoShield.begin();
    servoShield.setPWMFreq(100);

    // calibrateButtons();

    // Motor setup
    motorShield.begin();

    leftMotor->setSpeed(targetSpeed);
    leftMotor->run(RELEASE);

    rightMotor->setSpeed(targetSpeed);
    rightMotor->run(RELEASE);

    // Gyro setup
    gyro.initialize();
    currentAngle = 0;
    currentTime = millis()/1000.0;
    lastTimeStraight = currentTime;
    updateCurrentAngle();

    // Limit switch setup
    pinMode(12, INPUT_PULLUP);
}

boolean reset = false;
int positionInPi = 0;

void loop()
{
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
        servoShield.setPWM(i, 0, calibratingValue); 
}

// void moveWheels(int myDelayTime, int myDirection)
// {
//     // Turn the motors on
//     leftMotor->run(myDirection);
//     rightMotor->run(myDirection);

//     Serial.println("forward");

//     // TODO: changed to elapsed time
//     // Go for .... ms
//     delay(myDelayTime);

//     // Turn the motors off
//     leftMotor->run(RELEASE);
//     rightMotor->run(RELEASE);
// }

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
        servoShield.setPWM(servoNumber, 0, pressingValueLeft);
        delay(250);
        servoShield.setPWM(servoNumber, 0, restingValueLeft);
    }
    else
    {
        servoShield.setPWM(servoNumber, 0, pressingValueRight);
        delay(250);
        servoShield.setPWM(servoNumber, 0, restingValueRight);
    }
}

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}

void resetState()
{
    for (int i = 0; i < 15; i++)
    {

        if (i == 2)
            servoShield.setPWM(i, 0, pressingValueRight);
        else if (i == 7)
            servoShield.setPWM(i, 0, pressingValueLeft);
        else if (1 > 12)
            servoShield.setPWM(i, 0, calibratingValue);
        // TODO: we aren't using servos 10 - 12, should we skip them in this loop?
        
    }

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
        zRotationTrim = calibrateGyroZ();
        // zRotationCalibration = 0;
        Serial.println("gyro vals");
        Serial.println(zRotationTrim);

        state = dropWings;
    }
    return;
}

void getToWallState()
{
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);

    while (!digitalRead(12))
    {
        Serial.println("Getting to the wall");

        // TODO: Gyro magic is not working. I suspect its due to the cop/total speed
        runMotorsWithGyro();

    }
        
    state = dropWallClaw;
    return;
}

void dropWallClawState()
{
    servoShield.setPWM(13, 0, degreesToPwm(0));
    delay(2000);
    state = pushButtons;
    return;
}
                                             
void dropWingsState()
{
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);

    servoShield.setPWM(14, 0, degreesToPwm(120));
    servoShield.setPWM(15, 0, degreesToPwm(50));
    delay(500);

    servoShield.setPWM(14, 0, calibratingValue);
    servoShield.setPWM(15, 0, calibratingValue);

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
    updateCurrentAngle();
    double proportional = currentAngle*3; // Proportional magnifier
    double integral = getIntegralComponent()*15; // Integral magnifier

    int speedDelta = proportional + integral;

    // Since the gyro is positive counterclockwise, we must speed up the left wheel to counter act
    int rightSpeed = targetSpeed + speedDelta + LR_MotorTrim;
    int leftSpeed = targetSpeed - speedDelta - LR_MotorTrim;

    // Limit cap and min
    if (leftSpeed > 255)
      leftSpeed = 255;
    else if (leftSpeed < 50)
      leftSpeed = 50;

    if (rightSpeed > 255)
      rightSpeed = 255;
    else if (rightSpeed < 50)
      rightSpeed = 50;

    leftMotor -> setSpeed(leftSpeed);
    rightMotor -> setSpeed(rightSpeed);
}

void updateCurrentAngle()
{
  double pastTime = currentTime;

  double angleVelocity = gyro.getRotationZ() + zRotationTrim;
  angleVelocity = angleVelocity / 131.0; // This value is defined by the gryo's scale and sensitivity

  currentTime = millis()/1000.0;
  double deltaTime = currentTime - pastTime;
  
  double deltaAngle = angleVelocity * deltaTime;

  // If delta is greater than and opposite, it will pass through 0
  if (currentAngle*deltaAngle < 0 && abs(deltaAngle) > abs(currentAngle)) 
  {
    lastTimeStraight = currentTime;
  }
  currentAngle = currentAngle + deltaAngle;
}

double getIntegralComponent()
{
  // Time since it was straight
  double deltaTime = (millis()/1000) - lastTimeStraight;
  Serial.println(deltaTime);
  return currentAngle * abs(currentAngle) * deltaTime;
}