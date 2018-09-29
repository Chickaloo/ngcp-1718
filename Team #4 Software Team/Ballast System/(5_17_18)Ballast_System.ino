#include <MS5837.h>

#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;

int solenoidInPin = 0; //will eventually be set to pin 1 (adds air in)
int solenoidOutPin = 0; //will eventually be set to pin 2 (sends air out)

int initialDepth = 0; //UUV will save the initial depth reading as reference point for changing depth readings.
int adjustingDepth = 0; //variable that changes frequently to accomodate changing depth of UUV.
int depthMin = 0; //minimum depth UUV can go.
int depthMax = 0; //maximum depth UUV can go.

//int maxDepth = 1; //set max depth as 1 meter

bool descend = false;
bool ascend = false;
  
bool missionTest = true; //variable for testing purposes.

void setup() 
{
  Serial.begin(9600);
  
  Serial.println("Starting");

  pinMode(3, INPUT);

  //if pin 3 recieves no power, automatically call pinChange method to raise the UUV above water.
  attachInterrupt(digitalPinToInterrupt(3), pinChange, CHANGE); //pin 3 is used to detect killswitch.
  
  Wire.begin();

  sensor.init();
  
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void loop() 
{
  barometerReadings();

  //delay(30000); //delay 30 seconds
  
  initialDepth = depthReading(); //depth in meters

  Serial.print("Depth is ");
  Serial.print(initialDepth);
  Serial.print(" meters");

  delay(5000); //delay 5 seconds

  adjustingDepth = depthReading();

  depthMin = initialDepth - 0.15; //minimum depth UUV can go is half a foot above initial depth reading.

  depthMax = 0.15 + initialDepth; //maximum depth UUV can go is half a foot below depth reading.

  //The range in which the UUV tries to maintain underwater is: (depthMin < initialDepth < depthMax)

  if(adjustingDepth < depthMin) //if submarine is too close to the surface of water, signal that sub needs to go deeper underwater
  {
    descend = true;
    ascend = false;

    pinMode(solenoidOutPin, OUTPUT); //enable the pin for solenoidOut @ pin 0
  }
  else if(adjustingDepth > depthMax) // if submarine is too deep underwater, signal that the sub needs to rise up a bit.
  {
    descend = false;
    ascend = true;

    pinMode(solenoidInPin, OUTPUT); //enable the pin for solenoidIn @ pin 0
  }
  else
  {
    descend = false;
    ascend = false;
  }

  if(descend) //go underwater, allow water to enter.
  {
    solenoidInPin = 1; 
    solenoidOutPin = 2;

    pinMode(solenoidInPin, OUTPUT); //enable the pin for solenoidIn
    pinMode(solenoidOutPin, OUTPUT); //enable the pin for solenoidOut
    
    digitalWrite(solenoidOutPin, HIGH); //switch solenoidOut ON (which releases air to sink UUV)
    delay(1000); // waits 1 second(s) for the solenoid to release air and allow UUV to sink

    digitalWrite(solenoidOutPin, LOW); //switch solenoidOut OFF.
  }

  delay(1000); //delay for 1 second(s)

  if(ascend) //if sub is signaled to rise to surface, allow air to enter
  {
    solenoidInPin = 1;
    solenoidOutPin = 2;
    
    digitalWrite(solenoidInPin, HIGH); //switch solenoidIn ON (which allows air to enter balloon to make UUV float)
    delay(1000); //waits 1 second(s) for solenoid to fill with air and surface.
    
    digitalWrite(solenoidInPin, LOW); //switch solenoidIn OFF.
  }
}

void barometerReadings()
{ 
  sensor.read();

  Serial.print("Pressure: "); 
  Serial.print(sensor.pressure()); 
  Serial.println(" mbar");
  
  Serial.print("Temperature: "); 
  Serial.print(sensor.temperature()); 
  Serial.println(" deg C");
  
  Serial.print("Depth: "); 
  Serial.print(sensor.depth()); 
  Serial.println(" m");

  Serial.print("Altitude: "); 
  Serial.print(sensor.altitude()); 
  Serial.println(" m above mean sea level");

  delay(3000);
}

void pinChange()
{
  solenoidInPin = 1;
  solenoidOutPin = 2;

  descend = false;
  ascend = true;
  
  digitalWrite(solenoidInPin, HIGH); //switch solenoidIn on (which allows air to enter balloon to make UUV float)
  delay(10000); //waits 10 second(s) for solenoid to fill with air and surface.
    
  digitalWrite(solenoidInPin, LOW); //switch solenoidIn OFF.
}

int depthReading()
{
  int depthNum = 0; //depth in meters

  sensor.read();
  depthNum = sensor.depth();

  return depthNum;
}

