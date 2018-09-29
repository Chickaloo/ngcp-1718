const int motorPin = 2;
const int sensorPin = 3;
const int buttonPin = 3;
int buttonState; 
int i;

// We'll also set up some global variables for the light level:
int lightLevel, high = 0, low = 1023;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT); // Set up the push button pin to be input
  pinMode(motorPin, OUTPUT);  // Set up the motor pin to be an output
  Serial.begin(9600); // Set up the serial port:
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin); // reads pushbutton state
  Serial.print(buttonState); // tells us what button state it's reading
  Serial.println(i);
  if (buttonState == LOW && i == 0) // if pushbutton is pressed
  {
    i = 1; // set counter
    delay(3000);  // wait 3 seconds for torpedo to be loaded
    lightLevel = analogRead(sensorPin); // reads the light level
    manualTune(); // manually change the range from light ot dark
    Serial.print(lightLevel); // tell us what light level it's reading
    Serial.print(" ");
    Serial.print(low);
    Serial.print(" ");
    Serial.println(high);
    while (lightLevel >= 75)  // while dark, turn the motor off
    {
     motorOff();
     Serial.println("motor off");
     lightLevel = analogRead(sensorPin);
     manualTune();
    }
    Serial.println("motor on");
    // program should exit while loop when light turns on
    // and activate the motor
    motorOn();
  }
  else
  {
    i = 0; // reset counter
  }
}

void manualTune()
{
  // As we mentioned above, the light-sensing circuit we built
  // won't have a range all the way from 0 to 1023. It will likely
  // be more like 300 (dark) to 800 (light). If you run this sketch
  // as-is, the LED won't fully turn off, even in the dark.
  
  // You can accommodate the reduced range by manually 
  // tweaking the "from" range numbers in the map() function.
  // Here we're using the full range of 0 to 1023.
  // Try manually changing this to a smaller range (300 to 800
  // is a good guess), and try it out again. If the LED doesn't
  // go completely out, make the low number larger. If the LED
  // is always too bright, make the high number smaller.

  // Remember you're JUST changing the 0, 1023 in the line below!

  lightLevel = map(lightLevel, 0, 1023, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);

} 

void motorOff()
{
  int offTime = 100; // milliseconds to turn the motor off

  digitalWrite(motorPin, LOW);  // turn the motor off
  delay(offTime);               // delay for offTime milliseconds
}

void motorOn()
{
  int onTime = 10000;  // 10 seconds to turn the motor on
  
  digitalWrite(motorPin, HIGH); // turn the motor on (full speed)
  delay(onTime);                // delay for onTime milliseconds
  digitalWrite(motorPin, LOW); // turn the motor off when time runs out
}

