#include <Servo.h>

// --- Location for all global variables. ---
// Global variables can be used in any functions.

// A servo, which controls the mouth
Servo myservo;  

// Set this to set the number of servings the cat is fed every time.
// A serving is one back and forth with the servo.
int numberOfServings = 4;

// Change in light level to trigger feeding on.
// Set this to something between 200 and 400, depending on how
// sensitive you want the feeder.
int triggerLightLevel = 250;

// Internal variable for the last light level.
int _lastLightLevel = 0;

// Daily Feeding Settings

// Set this to turn on and off daily feeding
bool shouldFeedRegularly = true;
int hoursBetweenFeeding = 1;

// Internal variable for the time between feedings
unsigned long _millisecondsSinceLastFeeding = 0;
unsigned long _lastMillis = 0;

// --- End Location for all global variables. ---


// --- Utility functions ---

// Call this to feed the cat!
// It has one input: The number of times it needs to go back and forth.
void feedCat() 
{
  Serial.println("MeowMuncher5000 Engaged! Feeding the meow-meow!");

  for(int i = 0; i < numberOfServings; i++) {
    // Rotate left to 30 degrees and wait 500 milliseconds
    myservo.write(30);
    delay(500);                           
    // Rotate right to 140 degrees and wait 500 milliseconds
    myservo.write(140);
    delay(500);                           
    // Rotate back and wait a second
    myservo.write(90);
    delay(1000);
  }
}

// --- End Utility functions ---

// --- SPECIAL FUNCTION ---
// The special function called setup(), that runs once every time
// the Arduino starts.
void setup() 
{
  // Setup the servo on PIN 9
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  // Set the position to closed (which is at 90 degrees) and wait a second
  myservo.write(90);
  delay(1000);                           

  // Setup light sensor on PIN A0
  pinMode(A0, INPUT);
  _lastLightLevel = analogRead(A0);
  
  //Initiate Serial communication to the computer.
  Serial.begin(9600);  
  // Tell the computer we're alive
  Serial.println("MeowMuncher5000 online!");
}

// --- SPECIAL FUNCTION ---
// The loop() function runs in a loop as long as the Arduino is on.
void loop() {

  // Read the light level.
  int lightLevel = analogRead(A0);
  // Get the absolute difference.
  int lightDifference = abs(lightLevel - _lastLightLevel);
  _lastLightLevel = lightLevel;

  // Debug information
  Serial.print("Light value = ");
  Serial.print(lightLevel);
  Serial.print(", difference = ");
  Serial.println(lightDifference);

  // Check if we need to feed the cat and reset the timer?
  // Light difference must be large and negative (like waving you hand at it)
  if (lightDifference > triggerLightLevel) 
  {
    // If the light value difference is big, feed the cat and wait 3 seconds (3000 milliseconds)
    feedCat();

    // Read the light level again so we're back to normal after a feeding.
    _lastLightLevel = lightLevel; 

    // This is now the new feed time, so reset the regular feed timer.
    _millisecondsSinceLastFeeding = 0;
  }

  // Check if we should do a regular feed?
  if (shouldFeedRegularly) 
  {
    unsigned long chipMilliseconds = millis();
    if(chipMilliseconds - _lastMillis > 0)
    {
      //If milliseconds have elapsed, add them to our count.
      unsigned long msChange = chipMilliseconds - _lastMillis;
      _millisecondsSinceLastFeeding = _millisecondsSinceLastFeeding + msChange;

      // Check if we should do a regular feeding?
      // Convert hoursBetweenFeeding to milliseconds
      long millisecondsUntilNextFeeding = (hoursBetweenFeeding * 60l * 60l * 1000l - _millisecondsSinceLastFeeding) - 3550000;
      if (millisecondsUntilNextFeeding < 0)
      {
        // Feed the kitteh!
        feedCat();

        //Reset the timer.
        _millisecondsSinceLastFeeding = 0;
      } else 
      {
        Serial.print("Waiting ");
        Serial.print(millisecondsUntilNextFeeding / 1000);
        Serial.println(" seconds until the next feeding...");
      }
    }
    _lastMillis = chipMilliseconds;
  }

  delay(500);
}