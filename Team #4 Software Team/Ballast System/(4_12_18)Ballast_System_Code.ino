#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;

void setup() {
  
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
  bool goUnderwater = false;
  bool isUnderwater = false;
  
  barometerReadings();

  depth = depthReading(); //depth in meters

  Serial.print("Depth is currently" + depth + " meters");
  
  if(depth < 0.2) //if submarine has depth of 0.2 meters or less, signal that sub is not underwater
  {
    isUnderwater = false;
    goUnderwater = true;
  }
  else
  {
    isUnderwater = true;
    goUnderwater = false;
  }

  if(goUnderwater) //if sub is signaled to go underwater, loosen ballast to allow water to enter.
  {
    //servo opens valves until depth reads 1 meter.
    
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

  delay(1000);
}

int depthReading()
{
  int depthNum = 0; //depth in meters

  sensor.read();
  depthNum = sensor.depth();

  return depthNum;
}

