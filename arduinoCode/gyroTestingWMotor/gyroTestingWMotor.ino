//Drive Straight w/Gyro (MPU-6050)
#include<Wire.h>
#include <Adafruit_MotorShield.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyZ;
float elapsedTime, currentTime, previousTime,angle = 0, target_angle,offset = 0;

//target speed to go straight.
int target_speed = 70;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); // our shield's address
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);

void setup(){
  //I2C com with gyro
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  //button
  pinMode(7,INPUT_PULLUP);

  //setup motors
  AFMS.begin();
  
  leftMotor -> setSpeed(0);
  leftMotor -> run(FORWARD);
  leftMotor -> run(RELEASE);
  
  rightMotor -> setSpeed(0);
  rightMotor -> run(FORWARD);
  rightMotor -> run(RELEASE);

  //calibrate offset
  offset = calibrate();
}

//calibrate by taking 1000 measurements and averaging them. Then, we will subtract that value from our measurements later. 
float calibrate(){
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  delay(500);
  float GyZ_avg=0;
  for(int x=0;x<1000;x++){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 14 registers
  GyZ=(Wire.read()<<8|Wire.read())/131.0;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  GyZ_avg=GyZ+GyZ_avg;
  }
  GyZ_avg = GyZ_avg/1000.0;
  return GyZ_avg;
}

void loop(){

  //wait for calibrate button
  if(digitalRead(7)==0){
    offset = calibrate();
  }
  
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

  //Start I2C transmission with gyro
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x47 (GYRO_ZOUT)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  GyZ=(Wire.read()<<8|Wire.read())/131.0;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) (16 bit number stored in 2 8 bit registers)
  //subtract offset 
  GyZ=GyZ-offset;
  //convert from rad/s to rad
  angle = angle + GyZ*elapsedTime;
  Serial.print("angle = "); Serial.println(angle);

  //turned to the left, we'll increase power to left wheel to compensate (and drop power to right)
  if(angle>0){
  //change constant based on what power step size works for your motor.
  int cop_speed = 11*angle;

  //combine target speed with the speed to compensate for angle
  int total_speed = cop_speed+target_speed;
  //make sure speed is 255 and less.
  if(total_speed>255){total_speed = 255;}
  leftMotor -> setSpeed(total_speed);
  leftMotor -> run(FORWARD);

  total_speed = target_speed - cop_speed;
  if(total_speed <0){total_speed = 0;}
  rightMotor -> setSpeed(total_speed);
  rightMotor -> run(FORWARD);
  }

  //vice versa
  else{
  int cop_speed = -11*angle;
  int total_speed = cop_speed+target_speed;
  if(total_speed>255){total_speed = 255;}
  rightMotor -> setSpeed(total_speed);
  rightMotor -> run(FORWARD);

  total_speed = target_speed - cop_speed;
  if(total_speed <0){total_speed = 0;}
  leftMotor -> setSpeed(total_speed);
  leftMotor -> run(FORWARD);
  }
}
