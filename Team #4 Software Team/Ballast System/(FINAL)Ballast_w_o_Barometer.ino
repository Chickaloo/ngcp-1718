#include <Wire.h>

int solenoidInPin = 1; //set to pin 1 (adds air in)
int solenoidOutPin = 2; //set to pin 2 (sends air out)
int controlPin = 3; //set to pin 3 (controls ascension and decension of UUV).

bool descend = false; //regarding pin 1
bool ascend = false; //regarding pin 2

void setup() 
{
  Serial.begin(9600);
  
  Serial.println("Starting");

  Wire.begin();

  pinMode(solenoidInPin, OUTPUT);
  digitalWrite(solenoidInPin, LOW); //start with pin 1 to low (off)

  pinMode(solenoidOutPin, OUTPUT);
  digitalWrite(solenoidOutPin, LOW); //start with pin 2 to low (off)

  pinMode(controlPin, OUTPUT);
  digitalWrite(solenoidInPin, LOW); //start with pin 3 to low (off)
}

void loop() 
{
  
  if(digitalRead(3) == HIGH) //if submarine is too close to the surface of water, signal that sub needs to go deeper underwater
  {
    descend = true;
    ascend = false;
  }
  else if(digitalRead(3) == LOW) // if submarine is too deep underwater, signal that the sub needs to rise up a bit.
  {
    descend = false;
    ascend = true;
  }

  if(descend) //go underwater, allow water to enter.
  {
    solenoidInPin = 1; 
    solenoidOutPin = 2;

    pinMode(solenoidInPin, OUTPUT); //enable the pin for solenoidIn
    pinMode(solenoidOutPin, OUTPUT); //enable the pin for solenoidOut

    digitalWrite(solenoidInPin, LOW); //switch solenoidIn OFF.
    digitalWrite(solenoidOutPin, HIGH); //switch solenoidOut ON (which releases air to sink UUV)
    //delay(1000); // waits 1 second(s) for the solenoid to release air and allow UUV to sink

    //digitalWrite(solenoidOutPin, LOW); //switch solenoidOut OFF.
  }

  delay(1000); //delay for 1 second(s)

  if(ascend) //if sub is signaled to rise to surface, allow air to enter
  {
    solenoidInPin = 1;
    solenoidOutPin = 2;

    pinMode(solenoidInPin, OUTPUT); //enable the pin for solenoidIn
    pinMode(solenoidOutPin, OUTPUT); //enable the pin for solenoidOut

    digitalWrite(solenoidOutPin, LOW);
    digitalWrite(solenoidInPin, HIGH); //switch solenoidIn ON (which allows air to enter balloon to make UUV float)
    delay(10000); //waits 1 second(s) for solenoid to fill with air and surface.
    
    //digitalWrite(solenoidInPin, LOW); //switch solenoidIn OFF.
  }
}

