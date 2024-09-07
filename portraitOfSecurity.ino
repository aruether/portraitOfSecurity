// Portait of security
// Arduino Uno program to move eyes behind a portrait like Scooby Doo shows
// Andrew Ruether

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// Arduino I/O pins for PIR sensors
#define leftPIRpin  12  // left PIR sensor
#define rightPIRpin 13  // Right PIR sensor
#define servoPin  9     // Servo pin

// Delay before making another move (in mS)
#define delayTime 1800000

// Servo values for different positions
#define leftPosition 5
#define rightPosition 80
#define neutralPosition 120

int pos = 0;    // variable to store the servo position
bool inNeutralPosition = false;   // Are the eyes in a neutral position?
unsigned long lastMoveTime = 0;   // millis value when the eyes were last moved


void setup() {

  // Set PIR sensor I/O pins to inputs
  pinMode(leftPIRpin, INPUT);     
  pinMode(rightPIRpin, INPUT);     

  // Set up servo by attaching it to an Arduino pin
  myservo.attach(servoPin);  

  Serial.begin(115200);

  // Do a start up routine to move the eyes
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
  myservo.write(120 + random(40));
  inNeutralPosition = true;
}

void loop() {

  // Read PIR (Passive Infra-Red) sensors
  bool leftSensor = digitalRead(leftPIRpin);
  bool rightSensor = digitalRead(rightPIRpin);

  // Print out status
  Serial.print("Left: ");
  Serial.print(leftSensor);
  Serial.print("\t");
  Serial.print("Right: "); 
  Serial.print(rightSensor);
  Serial.print("\t");
  Serial.print("Last move time: "); 
  Serial.print(lastMoveTime);
  Serial.print("\t");
  Serial.print(millis());
  Serial.print(" ");
  Serial.println(delayTime);  

  // If left sensor was just triggered, move left
  if (leftSensor && (millis() - lastMoveTime) > delayTime) {
    myservo.write(leftPosition + random(5));  // Move to a left position
    lastMoveTime = millis();
    inNeutralPosition = false;
    Serial.println("Left motion detected");
  } else if (rightSensor && (millis() - lastMoveTime) > delayTime) {
    // If right sensor was just triggered, move to center
    myservo.write(rightPosition + random(10));
    lastMoveTime = millis();
    inNeutralPosition = false;
    Serial.println("Right motion detected");
  } else if (!inNeutralPosition && !leftSensor && !rightSensor && ((millis() - lastMoveTime) > 2*delayTime)) {
    // If nothing is going on for while, move to a neutral position.
    myservo.write(neutralPosition + random(40));
    lastMoveTime  = millis();
    inNeutralPosition = true;
    Serial.println("Move to neutral position");
  }
  delay(100);
}

