const int trigPin = 2; //Aduino pin 2
const int echoPin = 4; //Aduino pin 4




const int LED = 12; //Aduino pin 13
const int LEDGreen = 13;
long duration, distance, inches, cm;
const int MaxDistance = 22; //10 cm
const int In1 = 3;      
const int In2 = 5;      
const int In3 = 6;      
const int In4 = 9;




void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED,OUTPUT);
  pinMode(LEDGreen,OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
}




void loop()
{
  // put your main code here, to run repeatedly:
 int speed1;
 int speed2;
 int Twomax;
 int DifDistance;
 while (1==1)
 {
  ping();
  distance = cm;
  Twomax = distance*10;
  if( distance > MaxDistance && distance > Twomax)
  {
    ping();
    distance = cm;
    digitalWrite(LEDGreen,HIGH);
    digitalWrite(LED, LOW);
    speed1 = 100;
    speed2 = 110;
    forward(speed1, speed2);
    ping();
    distance = cm; 
   // delay (distance *2);
  }
  else if ( distance > MaxDistance && distance < Twomax)
  {
    ping();
    distance = cm;
    digitalWrite(LEDGreen,HIGH);
    digitalWrite(LED,LOW);
    speed1 = 100; 
    speed2 = 110;
    forward(speed1, speed2);
    delay (1000);
    stopNow();
  }
  else
  {
    digitalWrite(LED,HIGH);
    digitalWrite(LEDGreen,LOW);
    speed1 = 0;
    speed2 = 100;
    turn(speed1, speed2);
    delay(1000);
    ping();
    distance = cm;
    DifDistance = MaxDistance + 10;
    if (distance < DifDistance)
    {
      speed1 = 0;
      speed2 = 100;
      turn(speed1, speed2);
      delay(500);
      turn(speed1, speed2);
      delay(500);
    }
  }
 }
}
void ping ()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);
  // convert the time into a distance
  inches = duration / 74 / 2;
  cm = duration / 29 /2;
  return;
}
void forward(int speed1, int speed2) //Makes wheels go forward at speed1 and speed2
{
analogWrite(In4, 0);
analogWrite(In3, speed1);
analogWrite(In2, 0);
analogWrite(In1, speed2);
}
void reverse (int speed1, int speed2) //Makes wheels go backward at speed1 and speed2
{
analogWrite(In4, speed1);
analogWrite(In3, 0);
analogWrite(In2, speed2);
analogWrite(In1, 0);
return;
}
void stopNow() //Makes wheels stop
{
analogWrite(In4, 0);
analogWrite(In3, 0);
analogWrite(In2, 0);
analogWrite(In1, 0);
return;
}
void turn(int speed1, int speed2)
{
  analogWrite(In4,0);
  analogWrite(In3,0);
  analogWrite(In2, 0);
  analogWrite(In1, speed2);
}
