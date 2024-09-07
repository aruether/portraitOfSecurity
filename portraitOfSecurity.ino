#include <Servo.h>

Servo myservo;  // create servo object to control a servo

#define leftPIRpin  12
#define rightPIRpin 13


int pos = 0;    // variable to store the servo position

bool inNeutralPosition = false;

unsigned long lastRightMoveTime = 0;
unsigned long lastLeftMoveTime = 0;

// Delay before making another move
unsigned long delayTime = 1800000;

void setup() {

  pinMode(leftPIRpin, INPUT);     
  pinMode(rightPIRpin, INPUT);     

  Serial.begin(115200);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
             
  delay(1000);
  myservo.write(180); 
  delay(1000);
  myservo.write(10);  
  delay(1000);
  myservo.write(90); 

  // Go to neutral position
  myservo.write(random(120,160));
  inNeutralPosition = true;
}




void loop() {

  bool leftSensor = digitalRead(leftPIRpin);
  bool rightSensor = digitalRead(rightPIRpin);

  Serial.print(leftSensor);
  Serial.print(" ");
  Serial.print(lastLeftMoveTime);
  Serial.print(" ");
  Serial.print(rightSensor);
  Serial.print(" ");
  Serial.print(lastRightMoveTime);
  Serial.print(" ");
  Serial.print(millis());
  Serial.print(" ");
  Serial.println(delayTime);  

  // If left sensor was just triggered, move left
  if (leftSensor && (millis() - lastLeftMoveTime) > delayTime) {
    myservo.write(10);
    lastLeftMoveTime = millis();
    inNeutralPosition = false;
    Serial.println("Left motion detected");
  } else if (rightSensor && (millis() - lastRightMoveTime) > delayTime) {
    // If right sensor was just triggered, move to center
    myservo.write(90);
    lastRightMoveTime = millis();
    inNeutralPosition = false;
    Serial.println("Right motion detected");
  } 

  // If nothing is going on for while, move to a neutral position.
  if (!inNeutralPosition && !leftSensor && !rightSensor && ((millis() - lastLeftMoveTime) > 2*delayTime) && ((millis() - lastRightMoveTime) > 2*delayTime)) {
    myservo.write(random(120,160));
    lastRightMoveTime = lastLeftMoveTime = millis();
    inNeutralPosition = true;
    Serial.println("Move to neutral position");
  }
  delay(100);
}

