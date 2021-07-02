/**
   @author Tripack McLovin (spamforpatrick@gmx.de)
   @brief  7-segment digital clock arduino program

           setting the clock is done now by just using the terminal (9600baud), sending:
           dYY-MM-DD HH:MM:SS - e.g.: "d16-12-19 13:56:09"
           or without date
           tHH:MM:SS - e.g.: "t18:55:39"

   @library any version of FastLED will be fine, thank you guys for the great work!

   @library credits to the ds3231 library goes also to Petre Rodan <petre.rodan@simplex.ro>
              found here: https://github.com/rodan/ds3231
*/
#include <FastLED.h>
#include <TimeLib.h>

#define PIN_LED     5   //data pin of the APA104 led
#define PIN_PHOTO   A0  //pin of the photodiode

#define BRIGTHNESS_HIGH 100
#define BRIGHTNESS_LOW  1

int bfac = 255; //brightness factor
//repeat the first color at the end!
#define COLAMNT 6


#define POS_MIN     22//0
#define POS_TENMIN  15//7
#define POS_DOTS1   14//14
#define POS_HOUR    7//16
#define POS_TENHOUR 0//23

#define NUM_LEDS    29

//time allready disassembled as digits (more memory but less calculations)
byte ss_dec;
byte ss_dig;
byte mm_dec;
byte mm_dig;
byte hh_dec;
byte hh_dig;


#define MODE_NORMAL     0

#define MINUTES_PER_DAY 60*24

CRGB setColor;
CRGB clockleds[NUM_LEDS];
CRGB clockleds_prep[NUM_LEDS];
CRGB leds[NUM_LEDS];
//colors



int update_run = 0;
int update_run_sub = 0;

//debugging
#define BUFF_MAX 128

#define UPDATES_PER_SECOND 10
CRGBPalette16 currentPalette;

TBlendType    currentBlending;




/**
   @brief assigning a number to the specified position in the strip
    if you not fine how the numbers look
    (e.g. the bottom bar on the 9, or the hook on the 7)
    just change digit_bits[]
    pattern:

      -4-
     3   5
      -6-
     2   0
      -1-

*/
byte digit_bits[] = {  0b01111110, //0
                       0b1000010, //1
                       0b0110111, //2//
                       0b1100111, //3//
                       0b1001011, //4
                       0b1101101, //5
                       0b1111101, //6
                       0b1000110, //7
                       0b1111111, //8
                       0b1101111, //9
                       0b0011111,//p//10
                       0b1011111,//a 11
                       0b0111001,//t 12
                       0b0111101,//e 13
                       0b1110000,//u 14
                       0b1110001,//o 15
                       0b1110011,//d 16
                       0b1011001,//h 17
                       0b1100010,//j 18
                       0b0111000,//l 19
                       0b0110001,//c 20
                       0b1111001,//b 21
                       0b0010001,//r 22
                       0b0000000,// 23 space
                    };

void setup() {
  Serial.begin(115200);
  //  Wire.begin();
  //  DS3231_init(DS3231_INTCN);
  //setSyncProvider( requestSync);  //set function to call when sync required
  setSyncProvider( requestSync);  //set function to call when sync required
  FastLED.addLeds<APA104, PIN_LED, GRB>(clockleds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGTHNESS_HIGH );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}



void loop() {

  //processSyncMessage();
  //calcColor();
  //calcBrightness();
  updateFromPrep();
  //ChangePalettePeriodically();
  showHhMm();
  dot_processing();


  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041611; // Jan 1 2013

  if (Serial.find("T")) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}


void showMmSs() {
  setNum(POS_MIN, ss_dig);
  setNum(POS_TENMIN, ss_dec);
  setNum(POS_HOUR, mm_dig);
  setNum(POS_TENHOUR, mm_dec);
}

void showHhMm() {
  if ( ss_dig + ss_dec == 0 ) {
    update_run = 0;
    update_run_sub = 0;
  }

  //if ( update_run <= 0 ) {
  //setting numbers to cockleds_prep

  mm_dig = (byte)( second() / 10);
  float mm_dec_f = ((second() / 10.0) - mm_dig) * 10;
  mm_dec = (byte)mm_dec_f;
  hh_dig = (byte)( minute() / 10);
  float hh_dec_f = ((minute() / 10.0) - hh_dig) * 10;
  hh_dec = (byte)hh_dec_f;
  hh_dig = hh_dig < 1 ? 23 : hh_dig;

  //Serial.println(String(hh_dig, DEC) +  String(hh_dec, DEC) + ":" + String(mm_dig, DEC) + "" + String(mm_dec, DEC));
  setNum_nopos(hh_dig, hh_dec, mm_dig, mm_dec);
  //}

}

byte rwgc;
CRGB rwg[] = {0xB0B0B0, 0xFF0000, 0x30A000};
byte rwgStartOffset = 0;

void setNum_nopos(byte a, byte b, byte c, byte d) {
  setNum(POS_TENHOUR, a);
  setNum(POS_HOUR, b);
  setNum(POS_TENMIN, c);
  setNum(POS_MIN, d);
}
void setNum(int digit, byte number) {
  static uint8_t startIndex = 0;

  byte mask = 0x01;
  for (int i = 0; i < 7; i++) {

    //setColor = ColorFromPalette( currentPalette, digit + i, BRIGTHNESS_HIGH, currentBlending);
    clockleds_prep[digit + i] = (mask & digit_bits[number]) ? leds[digit + i] : CRGB::Black;
    clockleds_prep[i].fadeLightBy( 64 );
    //if (mask & digit_bits[number] != clockleds[i].) {
    //   clockleds_prep[digit + i] = CRGB::White;
    //}


    mask <<= 1;
  }
}
void updateFromPrep() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + random(1, 5); /* motion speed */
  FillLEDsFromPaletteColors(startIndex);
  byte mask = 0x01;
  for (byte i = 0; i < NUM_LEDS; i++) {
    if ( i == POS_DOTS1) i ++;

    //clockleds[i] = clockleds_prep[i] ? leds[i] : CRGB::Black;
    //clockleds[i] = clockleds_prep[i] ? leds[i] : clockleds_prep[i];
    clockleds[i] = clockleds_prep[i];
    //mask <<= 1;
    //clockleds[i] = clockleds_prep[i];
    //clockleds[i].nscale8(bfac);
  }

}

uint8_t colorIndex;
void calcColor() {
  //interpolating colors over the day
  /*
    int i = minuteofday / minutesperblock;
    int a = minuteofday - (i * minutesperblock);
    int b = minutesperblock - a;
    float fa = (float)a / (float)minutesperblock;
    float fb = (float)b / (float)minutesperblock;

    colA = cols[i];
    colA.nscale8(fb * 255);
    i++;
    if (i >= COLAMNT) i = 0;
    colB = cols[i];
    colB.nscale8(fa * 255);
    setColor = colA + colB;
  */
  //uint8_t brightness = 255;
  //setColor = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
  //colorIndex++;
  //setColor=CRGB::White;
  //setColor = CHSV( random8(), 255, random8());
}


int readout = 512;
void calcBrightness() {
  /*
    //lowpass filtering
    int r = analogRead(A0);
    if (abs(r - readout) > 90) { //jump if the input changes in large steps
      readout = r;
    } else {
      readout = (readout * 7 + r) >> 3; // 7/8 old and 1/8 new value
    }
    //translate ADC to brightness range
    bfac = map( BRIGHTNESS_ADC_LOW, BRIGHTNESS_ADC_HIGH, BRIGHTNESS_LOW, BRIGTHNESS_HIGH, readout);
    //clamp
    if (bfac < BRIGHTNESS_LOW) bfac = BRIGHTNESS_LOW;
    else if (bfac > BRIGTHNESS_HIGH) bfac = BRIGTHNESS_HIGH;
  */
  bfac = BRIGTHNESS_HIGH;
}

//byte ucount = 0;
void dot_processing() {
  /*
    ucount++;
    if (ucount > 25) ucount = 0;

    if (ucount == 0) {
    clockleds[POS_DOTS1  ] = setColor;
    clockleds[POS_DOTS1  ].nscale8(bfac);
    }
    clockleds[POS_DOTS1  ].nscale8(0xF8);
    clockleds[POS_DOTS1 + 1] = clockleds[POS_DOTS1  ];
  */
  if ( (ss_dec % 2) == 0) {
    clockleds[POS_DOTS1  ] = setColor;
    clockleds[POS_DOTS1  ].nscale8(bfac);
  } else {
    clockleds[POS_DOTS1  ].nscale8(0xF8);
    clockleds[POS_DOTS1] = clockleds[POS_DOTS1  ];
  }
}




void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 100;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 10;
  }
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
  }
}
void setNum_ce(int digit, byte number) {
  if (number >= 0 && number <= 9) {
    byte mask = 0x01;
    for (int i = 0; i < 7; i++) {
      clockleds_prep[digit + i] = (mask & digit_bits[number]) ? ( rwg[rwgc++] ) : CRGB::Black;
      if (rwgc > 2) rwgc = 0;
      mask <<= 1;
    }
  }
}


/**
   @brief this is summer/wintertime-determination

   (this is only works for the rules applied for the Mid-European time - change on the last sunday in march
   from 0200 to 0300 and last sunday in october from 0300 to 0200

   one could condense the formulas, but this is as entropic driven search the most efficient (lesser events are deeper)

   the summertime-flag is used to compensate the rtc time with the visible time

*/



char recv[BUFF_MAX];
byte incnt = 0;

time_t requestSync()
{
  Serial.write(7);
  return 0; // the time will be sent later in response to serial mesg
}

