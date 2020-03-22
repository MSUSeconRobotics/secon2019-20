#include "robot230.h"

State state = start;

void setup()
{
   //I2C com with gyro
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

    Serial.begin(115200);
    Serial.println("Robot230 Competition code");

    // Servo setup
    servoShield.begin();
    servoShield.setPWMFreq(100);

    calculateServoValues();

    // calibrateButtons();
    resetState();

    // Motor setup
    motorShield.begin();

    leftMotor->setSpeed(targetSpeed);
    leftMotor->run(RELEASE);

    rightMotor->setSpeed(targetSpeed);
    rightMotor->run(RELEASE);

    // Gyro setup
    //gyro.initialize();

    // Limit switch setup
    pinMode(12, INPUT_PULLUP);
    pinMode(0, INPUT_PULLUP);
    motors[0] = leftMotorSpeed;
    motors[1] = rightMotorSpeed;
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

void pressButton(int servoNumber)
{
    servoShield.setPWM(servoNumber, 0, pushingValue[servoNumber]);
    delay(pushingDelay);
    servoShield.setPWM(servoNumber, 0, restingValue[servoNumber]);
    delay(pullBackDelay);
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
            servoShield.setPWM(i, 0, pushingValue[14]);
        else if (i == 7)
            servoShield.setPWM(i, 0, pushingValue[15]);
        else if (i > 12)
            servoShield.setPWM(i, 0, calibratingValue);
        // TODO: we aren't using servos 10 - 12, should we skip them in this loop?
        else
        {
            servoShield.setPWM(i, 0, calibratingValue);
        }
    }

    reset = true;
}

void startState()
{
    if (digitalRead(0))
    {
        while (digitalRead(0))
        {
            Serial.println("Waiting for RELEASE");
            state = getToWall;
        }
        state = getToWall;
        endingTime = millis() + 181000;
    }
    
    return;
}

void getToWallState()
{
    currentTime = millis();
    while (digitalRead(12)==1)
    {
        driveStraight(motors);
        leftMotor->setSpeed(motors[0]);
        rightMotor->setSpeed(motors[1]);
        leftMotor->run(FORWARD);
        rightMotor->run(FORWARD);
    }
  
    state = dropWings;
    return;
}

//GYRO FUNCTIONS!!!
float getAngle(){
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

  //Start I2C transmission with gyro
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x47 (GYRO_ZOUT)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  GyZ=(Wire.read()<<8|Wire.read())/200;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) (16 bit number stored in 2 8 bit registers)
  //subtract offset 
  GyZ=GyZ-offset;
  //convert from rad/s to rad
  angle = angle + GyZ*elapsedTime;
  return angle;
  //Serial.print("angle = "); Serial.println(angle);
  delay(2);
}

int driveStraight(int * motors){
  angle = getAngle();

  //calulate integral value (error over time correction)
  if(abs(angle)<acceptableAngle){
    lastStraight = millis();
    iValue = 0;
  }
  else{
    iValue = millis()-lastStraight;
    iValue = iValue * iModifier;
    if(angle < 0) iValue = -iValue;
  }

  
  //"saturation" add with the 8 bit unsigned motor speeds. Add integral value as well. 
  leftMotorSpeed = constrain(targetSpeed + angle * pValue + iValue ,0 ,255);
  rightMotorSpeed = constrain(targetSpeed - (angle * pValue) - iValue, 0,255);
  
  motors[0] = leftMotorSpeed;
  motors[1] = rightMotorSpeed;
  
  Serial.print("Left Motor: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" Right Motor: ");
  Serial.print(rightMotorSpeed);
  Serial.print(" angle: ");
  Serial.println(angle);
  //return [leftMotor,rightMotor];
}//END OF GYRO FUNCTIONS



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

    servoShield.setPWM(14, 0, pushingValue[14]);
    servoShield.setPWM(15, 0, pushingValue[15]);
    servoShield.setPWM(12, 0, pushingValue[12]);
    delay(500);

    servoShield.setPWM(14, 0, calibratingValue);
    servoShield.setPWM(15, 0, calibratingValue);

    state = dropWallClaw;
    return;
}

void pushButtonsState()
{
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);

    int digit = piDigit(positionInPi++);
    pressButton(digit);

    if (millis() >= endingTime)
        state = end;
    // else if (!digitalRead(12))
    //     state = getToWall;
    return;
}

void endState()
{
    resetState();
    state = 10;
    return;
}

int piDigit(int posn) {
   return pgm_read_byte_near(pi + posn) - '0';
}

void calculateServoValues()
{
  for (int i = 0; i <= 15; i++)
    {
        if (i == 0 || i == 1 || i == 3 || i == 4 || i == 7 || i == 14) // Buttons 0 through 4 and buttons 5 through 9 are oriented in two different directions
        {
            // Note that order matters here
            pushingValue[i] = degreesToPwm(calibrationAngle + restingBaseline + restingValue[i] + pushingBaseline + pushingValue[i]);
            restingValue[i] = degreesToPwm(calibrationAngle + restingBaseline + restingValue[i]);
        }
        else
        {
            // Note that order matters here
            pushingValue[i] = degreesToPwm(calibrationAngle - restingBaseline - restingValue[i] - pushingBaseline - pushingValue[i]);
            restingValue[i] = degreesToPwm(calibrationAngle - restingBaseline - restingValue[i]);
        }
    }
}