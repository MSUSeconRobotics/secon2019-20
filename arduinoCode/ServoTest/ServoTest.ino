/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
String inString;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int restingPos = 60;
int pressingPos = 150;

void setup() {
  myservo.attach(8);  // attaches the servo on pin 9 to the servo object
//  myservo.write(0);
  Serial.begin(9600);
  Serial.println("yooo");
}

void loop() {
//  Controls based on input-------
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '-') {
      pos = inString.toInt();
      Serial.print("Finished int: ");
      Serial.println(pos);
      inString = "";
      myservo.write(pos);
    }
  }

//Swaying back and forth------
//myservo.write(0);
//delay(800);
//myservo.write(180);
//delay(1000);

//  delay(3000);
//  pressButton();

}




void pressButton(){
  myservo.write(pressingPos);
  delay(500);
  myservo.write(restingPos);
}


