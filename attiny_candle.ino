//Attiny45 with 3 LED (2 yellow and 1 red)
//mounted on separated PWM pin and with one 85ohm resistor
//Cunsumption at 4V : 20mA ON and 0.1µA in sleep mode
//Autonomy with li-ion 3.7 1800mAh : 90H ON or 2000 years in sleep mode

#include <avr/sleep.h>

const int p_red1 = 0;
const int p_yel2 = 1;
const int p_yel3 = 4;
const int p_bt = 3; // pin2 (PB3)
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B}; // PB0 PB1 PB4
unsigned long lifetime = 0;

int raiseBr[] = {1, 1, 1};
int br[] = {150, 150, 150};
int brMax[] = {255, 255, 255};
int brMin[] = {100, 100, 100};
int incr[] = {1, 1, 1};
void SetBr (int led, int intensity) {
  *Port[led] = 255 - intensity;
}

// Utility macros
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

void setup() {
  SetBr(0, 0);
  SetBr(1, 0);
  SetBr(2, 0);
  adc_disable(); // ADC uses ~320uA
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  randomSeed (analogRead (3)); // Randomise
  pinMode(p_red1, OUTPUT);
  pinMode(p_yel2, OUTPUT);
  pinMode(p_yel3, OUTPUT);
  pinMode(p_bt, INPUT);

  TCCR0A = 3 << COM0A0 | 3 << COM0B0 | 3 << WGM00;
  TCCR0B = 0 << WGM02 | 3 << CS00; // Optional; already set
  // Configure counter/timer1 for fast PWM on PB4
  GTCCR = 1 << PWM1B | 3 << COM1B0;
  TCCR1 = 3 << COM1A0 | 7 << CS10;
}

void loop() {
  // For loop sets the number of pulses and repeats these pulses
  if (digitalRead(p_bt) == 1)
  {
    delay(200);
    enterSleep();
    SetBr(0, 100);
    SetBr(1, 100);
    SetBr(2, 100);
    delay(400);
  }
  else
  {
    candle(0);
    candle(1);
    candle(2);
  }
  if (lifetime++ > 1080000) {
    enterSleep(); // 1080000 = 3H with 10ms delay
  }
  delay(10); // Adds a delay between pulse sets
}

void enterSleep(void)
{
  SetBr(0, 0);
  SetBr(1, 0);
  SetBr(2, 0);
  delay(400);
  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT3);
  sleep_enable();
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep
  lifetime = 0;
  //  cli();                                  // Disable interrupts
  //  PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
  //  sleep_disable();                        // Clear SE bit
}

ISR(PCINT0_vect) {
  // This is called when the interrupt occurs, but I don't need to do anything in it
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


