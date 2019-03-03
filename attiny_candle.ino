/*
  LED Candle
  
  Makes an LED pulse randomly to simulate a candles flame. Takes a series of values and randomly
  generates sets of pulses with varied brightness and frequency within these values.
 
  "by A Green for x2Jiggy.com"
  
 */

int ledPin = 0; // Pin the LED is connected to
int ledPin2 = 1; // Pin the LED is connected to

int pulseMin = 1; // Minimum number of pulses in each set
int pulseMax = 5; // Maximum number of pulses in each set

int brightMin = 75; // Baseline LED brightness. Cannot exceed 128.

int minDelay = 100; // Minimum delay between pulse sets (ms)
int maxDelay = 2000; // Maximum delay between pulse sets (ms)

void setup() {                
  
  randomSeed (analogRead (3)); // Randomise
  pinMode(ledPin, OUTPUT); // Sets LED Pin to be an output 
  pinMode(ledPin2, OUTPUT); // Sets LED Pin to be an output 
  
}

void loop() {

  // For loop sets the number of pulses and repeats these pulses
  int incr =  random(1, 10);
  for (int x = random(pulseMin, pulseMax); x > 0; x-- ) { 
    
    int bright = 244 - random(96); // Sets a random maximum brightness level for this pulse
  
    // For loop raises the brightness of the LED from minimum to maximum value
  
    for (int y = brightMin; y < bright ; y+=incr) {
      incr =  random(1, 4);
      analogWrite(ledPin, y);
      analogWrite(ledPin2, y);
      delay(3);
      
    }
    
    // For loop lowers the brightness of the LED from maximum to minimum value
    
    for (int y = bright; y > brightMin; y-=incr) {
      incr =  random(1, 4);
      analogWrite(ledPin, y);
      analogWrite(ledPin2, y);
      delay(3);
      
    }
    
    delay(10); // Adds a delay between pulses to make them more visible
    
  }

  analogWrite(ledPin, brightMin);
  analogWrite(ledPin2, brightMin);
  delay(random(minDelay,maxDelay)); // Adds a delay between pulse sets
    
}
 

