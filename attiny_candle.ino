/*
  LED Candle

  Makes an LED pulse randomly to simulate a candles flame. Takes a series of values and randomly
  generates sets of pulses with varied brightness and frequency within these values.

  "by A Green for x2Jiggy.com"

*/

int p_red1 = 0;
int p_red2 = 1;
int p_yel3 = 4;
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

int pulseMin = 1; // Minimum number of pulses in each set
int pulseMax = 5; // Maximum number of pulses in each set

int brightMin = 75; // Baseline LED brightness. Cannot exceed 128.

int minDelay = 100; // Minimum delay between pulse sets (ms)
int maxDelay = 2000; // Maximum delay between pulse sets (ms)

void setup() {

  randomSeed (analogRead (3)); // Randomise
  pinMode(p_red1, OUTPUT);
  pinMode(p_red2, OUTPUT);
  pinMode(p_yel3, OUTPUT);
  
  TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 | 3<<CS00; // Optional; already set
  // Configure counter/timer1 for fast PWM on PB4
  GTCCR = 1<<PWM1B | 3<<COM1B0;
  TCCR1 = 3<<COM1A0 | 7<<CS10;
  SetColour(0, 0);
  SetColour(1, 0);
  SetColour(2, 0);
}

void SetColour (int led, int intensity) {
  *Port[led] = 255-intensity;
}


void loop() {
    // For loop sets the number of pulses and repeats these pulses
    int incr =  random(1, 10);
    for (int x = random(pulseMin, pulseMax); x > 0; x-- ) {

      int bright = 255 - random(96); // Sets a random maximum brightness level for this pulse

      // For loop raises the brightness of the LED from minimum to maximum value
      for (int y = brightMin; y < bright ; y += incr) {
        incr =  random(1, 4);
        SetColour(0, y);
        SetColour(1, y);
        SetColour(2, y);
        delay(3);
      }
      // For loop lowers the brightness of the LED from maximum to minimum value
      for (int y = bright; y > brightMin; y -= incr) {
        incr =  random(1, 4);
        SetColour(0, y);
        SetColour(1, y);
        SetColour(2, y);
        delay(3);
      }
      delay(10); // Adds a delay between pulses to make them more visible
    }
    SetColour(0, brightMin+50);
    SetColour(1, brightMin+50);
    SetColour(2, brightMin+50);
    delay(random(minDelay, maxDelay)); // Adds a delay between pulse sets

}


