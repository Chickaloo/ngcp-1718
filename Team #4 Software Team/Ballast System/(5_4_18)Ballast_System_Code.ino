#include <Wire.h>
//#include <Servo.h>
#include "MS5837.h"

MS5837 sensor;
//Servo myservo;

int solenoidInPin = 1;
int solenoidOutPin = 2;

void setup() {
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  pinMode(solenoidInPin, OUTPUT); //enable the pin for solenoidIn
  pinMode(solenoidOutPin, OUTPUT); //enable the pin for solenoidOut

  Serial.begin(9600);
  
  Serial.println("Starting");
  
  Wire.begin();

  sensor.init();
  
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void loop() 
{
  int depth = 0;
  int maxDepth = 1; //set max depth as 1 meter

  bool missionStart = false;
  bool missionEnd = false;
  
  barometerReadings();

  depth = depthReading(); //depth in meters

  Serial.print("Depth is ");
  Serial.print(depth);
  Serial.print(" meters");
  
  if(depth < 0.18) //if submarine has depth of 0.18 meters (0.59ft) or less, signal that sub is not underwater
  {
    missionStart = true;
    missionEnd = false;
  }
  else
  {
    missionStart = false;
    missionEnd = false;
  }

  if(missionStart) //On mission start, go underwater, allow water to enter.
  {
    digitalWrite(solenoidOutPin, HIGH); //switch solenoidOut on (which releases air to sink UUV)
    delay(5000); // waits 5 seconds for the solenoid to release air and allow UUV to sink

    digitalWrite(solenoidOutPin, LOW); //switch solenoidOut OFF.
  }

  //add code for when mission is finished and signal is received, set missionEnd = true;

  if(missionEnd) //if sub is signaled to rise to surface, allow air to enter
  {
    digitalWrite(solenoidInPin, HIGH); //switch solenoidIn on (which allows air to enter balloon to make UUV float)
    delay(5000); //waits 5 seconds for solenoid to fill with air and surface.
    
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

int depthReading()
{
  int depthNum = 0; //depth in meters

  sensor.read();
  depthNum = sensor.depth();

  return depthNum;
}

