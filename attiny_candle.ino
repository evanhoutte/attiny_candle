
int p_red1 = 0;
int p_red2 = 1;
int p_yel3 = 4;
int pin[] = {0,1,4};
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

int raiseBr[] = {1,1,1};
int br[] = {150,150,150};
int brMax[] = {255,255,255};
int brMin[] = {100,100,100}; 
int incr[] = {1,1,1}; 
void SetBr (int led, int intensity) {
  *Port[led] = 255 - intensity;
}

void setup() {

  randomSeed (analogRead (3)); // Randomise
  pinMode(p_red1, OUTPUT);
  pinMode(p_red2, OUTPUT);
  pinMode(p_yel3, OUTPUT);

  TCCR0A = 3 << COM0A0 | 3 << COM0B0 | 3 << WGM00;
  TCCR0B = 0 << WGM02 | 3 << CS00; // Optional; already set
  // Configure counter/timer1 for fast PWM on PB4
  GTCCR = 1 << PWM1B | 3 << COM1B0;
  TCCR1 = 3 << COM1A0 | 7 << CS10;
  SetBr(0, 0);
  SetBr(1, 0);
  SetBr(2, 0);
}

void loop() {
  // For loop sets the number of pulses and repeats these pulses
  candle(0);
  candle(1);
  candle(2);
  delay(10); // Adds a delay between pulse sets
}

void candle(int id)
{
  
  if (raiseBr[id] == 1)
  {
    if ( br[id] > brMax[id])
    {
      brMax[id] = random(160, 245);
      raiseBr[id] = 0;
      incr[id] =  random(1, 10);
    }
    br[id] += incr[id];
  }
  else
  {
    if ( br[id] < brMin[id])
    {
      brMin[id] = random(75, 150);
      raiseBr[id] = 1;
      incr[id] =  random(1, 10);
    }
    br[id] -= incr[id];
  }
  SetBr(id, br[id]);
}


