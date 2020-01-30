#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MotorShield.h>

double degreesToPwm(int degree)
{
    return (725 / 180 * degree + 275);
}

// Servo shield setup: called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// You can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Motor Shield Setup:
// AFMS: Motorshield default address is 0x60, but we have some in lab that are 0x61
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

// This will be iterated through to press buttons. Each number corresponds to a servo
String ordering = "0110";
// String ordering = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378";

// Our degree constants
int calibrationPos = 90; //Set to 90 when attaching the arm
int deltaDegrees = 30;
int restingPos = 80;
int pressingPosLeft = restingPos + deltaDegrees;
int pressingPosRight = restingPos - deltaDegrees;

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

    calibrateButtons();

    // Motor setup
    AFMS.begin();

    leftMotor->setSpeed(0);
    leftMotor->run(FORWARD);
    leftMotor->run(RELEASE);

    rightMotor->setSpeed(0);
    rightMotor->run(FORWARD);
    rightMotor->run(RELEASE);

    leftMotor->setSpeed(150);
    rightMotor->setSpeed(150);

    // Limit switch setup
    pinMode(12, INPUT_PULLUP);
}

boolean running = true;
int i = 0;

void loop()
{
    if(!digitalRead(12))
    {
        leftMotor->run(FORWARD);
        rightMotor->run(FORWARD);

        while (!digitalRead(12))
        {
            Serial.println("Getting back to the wall");
        }

        leftMotor->run(RELEASE);
        rightMotor->run(RELEASE);
    }

    if (running)
    {
        String charOfOrdering = ordering.substring(i, i + 1);
        pressButton(charOfOrdering.toInt());
        
        delay(1000);

        if (i >= ordering.length())
            running = false;

        i++;
    }
}

void calibrateButtons()
{
    for (int i = 0; i <= 10; i++)
    {
        pwm.setPWM(i, 0, restingValue); //   UNCOMMENT to put arm at furthest back position
    //    pwm.setPWM(i,0,pressingValueLeft); //   UNCOMMENT to put arm at pressed position
    }
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
    // TODO: change to elapsed time
    if (servoNumber <= 4) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
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
