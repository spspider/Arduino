// AC dimmer
// The hardware timer runs at 16MHz. Using a
// divide by 256 on the counter each count is 
// 16 microseconds.  1/2 wave of a 60Hz AC signal
// is about 520 counts (8,333 miliseconds).
// 1/2 wave of a 50 Hz signal is 625 counts
/*
Pin    |  Interrrupt # | Arduino Platform
---------------------------------------
2      |  0            |  All -But it is INT1 on the Leonardo
3      |  1            |  All -But it is INT0 on the Leonardo
18     |  5            |  Arduino Mega Only
19     |  4            |  Arduino Mega Only
20     |  3            |  Arduino Mega Only
21     |  2            |  Arduino Mega Only
0      |  0            |  Leonardo
1      |  3            |  Leonardo
7      |  4            |  Leonardo
The Arduino Due has no standard interrupt pins as an iterrupt can be attached to almosty any pin. 

In the program pin 2 is chosen
*/


#include <io.h>
#include <interrupt.h>

#define ZEROCROSS 2  //zero cross detect
#define TRIAC 3    //triac gate
#define PULSE 4   //trigger pulse width (counts)
int dim=483;

void setup(){

  // set up pins
  pinMode(ZEROCROSS, INPUT);       // zero cross detect
  digitalWrite(ZEROCROSS, HIGH);  // enable pull-up resistor
  pinMode(TRIAC, OUTPUT);       // triac gate control

  // set up Timer1 
  //(see ATMEGA 328 data sheet pg 134
  OCR1A = 100;      // initialize the comparator  compared with TCNT1
  TIMSK1 = 0x03;    // enable comparator A and overflow interrupts  
            // 0x03=0b11 =OCIE1A and TOIE1
            // OCIE1A Timer/Counter1 Output Compare A Match interrupt enable. Interrupt set
            // TOIE1 Timer/Counter1 Overflow interrupt is enabled.
  TCCR1A = 0x00;    // timer control registers set for
  TCCR1B = 0x00;    // normal operation, timer disabled


  // set up zero crossing interrupt
  attachInterrupt(0,zeroCrossingInterrupt, RISING);    
}  

//Interrupt Service Routines

void ICACHE_RAM_ATTR zeroCrossingInterrupt(){ //zero cross detect   
  TCCR1B=0x04; //start timer with divide by 256 input 16us tics  4=0b100 =CS12:1, CS11:0, CS10:0 table 16.5
  TCNT1 = 0;   //reset timer - count from zero
}

ISR(TIMER1_COMPA_vect){     // comparator match. overflow werd bereikt als de waarde van 'i'  wordt gematched
  digitalWrite(TRIAC,HIGH);   // Triac will be set high
  TCNT1 = 65536-PULSE;        // trigger pulse width telt 4 periodes van 16uS
}

ISR(TIMER1_OVF_vect){       // timer1 overflow wordt bereikt na 4x16us
  digitalWrite(TRIAC,LOW);    // turn off triac gate
  TCCR1B = 0x00;              // disable timer stopd unintended triggers
}

void loop(){ // sample code to exercise the circuit

dim--;
OCR1A = i;        //set the compare register brightness desired. delay to on value
if (dim<65){dim=483;} // 1 msec tot 7.726 msec       
delay(15);                             

}
