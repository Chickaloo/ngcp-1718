#include <MS5837.h>
#include <Wire.h>
#include <Servo.h>
//#include "MS5837.h"

MS5837 sensor;
Servo myservo;

int pos = 0;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
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

  Serial.print("Depth is ");
  Serial.print(depth);
  Serial.print(" meters");
  
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
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }

    delay(2000);  //delay for 2 seconds

    //closes servo valves.
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    myservo.detach(); //prevents servo from opening up again.
  }
}
  //if(

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

