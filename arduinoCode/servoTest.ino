/*************************************************** 
  This is an example for our keyestudio 16-channel PWM & Servo driver
  PWM test - this will drive 16 PWMs in a 'wave'

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  keyestudio invests time and resources providing this open source code, 
  please support keyestudio and open-source hardware by purchasing 
  products from keyestudio !
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

String userInput = "";

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  Serial.println("Input values 1-4095");

  pwm.begin();
  pwm.setPWMFreq(100);    
}

void loop() {
  long pwmValue = 1;
  while (Serial.available())
  {
    userInput = Serial.readString();
    Serial.print("Recieved: ");
    Serial.println(userInput);
    
    pwmValue = userInput.toInt();
    if (pwmValue <= 0 || pwmValue > 4095)
    {
      Serial.println("-- BAD INPUT");
    }
    else
    {
      Serial.print("Attempting");
      pwm.setPWM(0,0,pwmValue);
    }
  }
}
