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
//1Mbm 128kBit!!!!
#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
//#include <Wire.h>
//#include <ds3231.h>
#include <TimeLib.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
//******************** CONFIGURATION *************************
//uncomment this for the large version with 6 digits,  let it uncommented for the 4 digits case
//#define HH_MM_SS
//comment this if you are not fine with the automatic DD:MM-HH:MM-MM:SS switching
#define DDMMHHMMSS_TOGGLE

//uncomment this if you use 2 leds per bar
//#define DUALBAR

#define PIN_LED     15   //data pin of the APA104 led
#define PIN_PHOTO   A0  //pin of the photodiode

//for our imperial friends (there is no am/pm indicator)
//#define TYPE_12H

//brigthness adjustments
//value of the photo value in darkest environment
#define BRIGHTNESS_ADC_LOW  20
//value in the brightest environment
#define BRIGHTNESS_ADC_HIGH 600
//max and min brightness
#define BRIGTHNESS_HIGH 10
#define BRIGHTNESS_LOW  1


//start time and date when uploading the code (this will also be used for winter/summer-time adjustment)
//it must be entered here as wintertime (normal),
//and can prevent you from using manual setting of time
#define SD_DD 20
#define SD_MM 12
#define SD_YY 16
#define ST_HH 18
#define ST_MM 00
#define ST_SS 00

//color definitions
//these are the colors followed through the day
//every n'th hour, a new color is reached, between these, the color is interpolated,
//this should help to get the time without precisly read it, especially during the night
//on APA104 this is RGB, please look at your type, it might be GRB or whatever
//starting at midnight, equally spaces through the day (e.g. 4 values will be 0, 6, 12 and 18 o'clock peak of that color)
CRGB cols[] = {0xFF0000, 0x0000FF, 0x00FF00, 0xFFFFFF, 0xFFFF80, 0xFF8000};
CRGB colA;
CRGB colB;
int bfac;//brightness factor
//repeat the first color at the end!
#define COLAMNT 6


//******************** implementation (nothing to change here) *************************
//positions of the numbers and dots along the strip
#ifdef HH_MM_SS
#define POS_SEC     30
#define POS_TENSEC  30
#define POS_DOTS1   14
#define POS_DOTS2   14
#define POS_HOUR    7
#define POS_TENHOUR 0

#else
#define POS_MIN     22//0
#define POS_TENMIN  15//7
#define POS_DOTS1   14//14
#define POS_HOUR    7//16
#define POS_TENHOUR 0//23

#endif
#define NUM_LEDS    29

//date
byte dd_dd;
byte dd_mm;
byte dd_yy;
bool summertime;

//time allready disassembled as digits (more memory but less calculations)
byte ss_dec;
byte ss_dig;
byte mm_dec;
byte mm_dig;
byte hh_dec;
byte hh_dig;

uint8_t bhigh;
uint8_t blow;

uint8_t nmode;
uint8_t dmode;
uint8_t color = 0;

#define MODE_NORMAL     0
#define MODE_CHRISTMAS  1
#define MODE_1STAPRIL   2
byte mode = MODE_1STAPRIL;

#define MINUTES_PER_DAY 60*24
int minuteofday;
int minutesperblock;

#define SYMBOL_DEG 10

CRGB setColor;
CRGB clockleds[NUM_LEDS];
CRGB clockleds_prep[NUM_LEDS];
//colors
#define colnum sizeof(cols)/sizeof(CRGB)
int colindex;

unsigned long lastSecUpdate;

int update_run = 0;
int update_run_sub = 0;

//debugging
#define BUFF_MAX 128



void load_clock_proretes(const char* jsonConfig) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  if (!root.success()) {
    Serial.println("Parsing fail");
    //    return false;

  }

  root.containsKey("bhigh") ? bhigh = root["bhigh"] : bhigh = 10;
  root.containsKey("blow") ? blow = root["blow"] : blow = 1;
  root.containsKey("nmode") ? nmode = root["nmode"] : nmode = 21;
  root.containsKey("dmode") ? dmode = root["dmode"] : dmode = 7;
  root.containsKey("color") ? color = root["color"] : color = 0;
  setMode();
}
void setup_clock() {
  //  Wire.begin();
  //  DS3231_init(DS3231_INTCN);
  //setSyncProvider( requestSync);  //set function to call when sync required
  FastLED.addLeds<WS2811, PIN_LED, GRB>(clockleds, NUM_LEDS);
  colindex = 1;



  minutesperblock = 240;

  update_run = 0;
  update_run_sub = 0;

  setMode();
  mode = 5;
}



void loop_clock() {

  //processSyncMessage();


  //int now = millis();

  //
  internal_increments();

  calcColor();

  calcBrightness();


  switch (mode) {
    case MODE_NORMAL:
      showHhMm();

      dot_processing();
      updateFromPrep();
      break;
    case MODE_CHRISTMAS:
      updateFromPrep();
      break;

    case MODE_1STAPRIL:
      //      proc_april();
      //      updateFromPrep();
      break;
    case 3://sync
      show_Sync_msg();
      updateFromPrep();
      break;
    case 4://net
      show_net_msg();
      updateFromPrep();
      break;
    case 5://conn
      show_conn_msg();
      updateFromPrep();
      break;
    default:
      mode = MODE_NORMAL;
      break;
  }
  // }
  FastLED.show();
  //delay(40); //all calculations needs around one milisecond, so this should be fine to not rely on interrupts
  if (millis() - lastSecUpdate > 1000) {
    //setMode();
    lastSecUpdate = millis();
  }
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041611; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}
void setMode() {
  if ( dd_mm == 12 && (dd_dd == 24) ) {
    mode = MODE_CHRISTMAS;
  } else if (dd_mm == 4 && dd_dd == 1) {
    mode = MODE_1STAPRIL;
  }
  else if (timeStatus() != timeSet) {
    mode = 4;//net
  }
  else if (status != WL_CONNECTED) {
    mode = 5;//conn
  }
  else if (timeStatus() == timeSet) {
    mode = MODE_NORMAL;
  }
}

void showMmSs() {

  setNum(POS_MIN, ss_dig);
  setNum(POS_TENMIN, ss_dec);
  setNum(POS_HOUR, mm_dig);
  setNum(POS_TENHOUR, mm_dec);
}
void show_net_msg() {
  setNum_ONE(26, 13, 12, 23);
}
void show_conn_msg() {
  setNum_ONE(20, 15, 26, 26);
}
void show_Sync_msg() {
  setNum(POS_TENHOUR, 24);//s
  setNum(POS_HOUR, 25);//y
  setNum(POS_TENMIN, 26);//n
  setNum(POS_MIN, 20);//c
}
void setNum_ONE(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  setNum(POS_TENHOUR, a);
  setNum(POS_HOUR, b);
  setNum(POS_TENMIN, c);
  setNum(POS_MIN, d);
}
void showHhMm() {
  if (( ss_dig + ss_dec == 0 ) || (second() % 2)) {
    update_run = 0;
    update_run_sub = 0;
  }

  if ( update_run <= 0 ) {
    //setting numbers to cockleds_prep

    mm_dig = (int)( minute() / 10);
    float mm_dec_f = ((minute() / 10.0) - mm_dig) * 10;
    mm_dec = (int)mm_dec_f;
    hh_dig = (int)( hour() / 10);
    float hh_dec_f = ((hour() / 10.0) - hh_dig) * 10;
    hh_dec = (int)hh_dec_f;
    hh_dig = hh_dig < 1 ? 23 : hh_dig;
    setNum(POS_TENHOUR, hh_dig);
    setNum(POS_HOUR, hh_dec);
    setNum(POS_TENMIN, mm_dig);
    setNum(POS_MIN, mm_dec);

    //setNum(POS_MIN, mm_dig);
    //setNum(POS_TENMIN, mm_dec);
    // setNum(POS_HOUR, hh_dig);
    // setNum(POS_TENHOUR, hh_dec);
  }

}

byte rwgc;
CRGB rwg[] = {0xB0B0B0, 0xFF0000, 0x30A000};
byte rwgStartOffset = 0;

void updateFromPrep() {
  //nscale8( clockleds, NUM_LEDS, 1);
  if ( update_run < NUM_LEDS ) {
    //update_run_sub++;
    //if ( update_run_sub > 1) {
    update_run_sub = 0;
    clockleds[update_run] = clockleds_prep[update_run];
    clockleds[update_run].nscale8(bfac);
    update_run++;
    if ( update_run == POS_DOTS1) update_run += 1;
    if (update_run >= NUM_LEDS) update_run = 0;
    //}
  }

}


void calcColor() {
  //interpolating colors over the day
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

  if (color == 0) {
    setColor = colA + colB;
  } else {
    setColor = CHSV(color, 255, bfac);
  }
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


  if ((hour() >= nmode) || (hour() <= dmode )) {
    bfac = blow;
  } else {
    bfac = bhigh;
  }

}

void internal_increments() {
  //incrementing numbers (least effords regarding computations
  ss_dig++;
  if (ss_dig > 9) {
    ss_dig = 0;
    ss_dec++;
    if (ss_dec > 5) { //new minute
      minuteofday++;
      ss_dec = 0;
      mm_dig++;
      if (mm_dig > 9) { //new ten minute
        mm_dig = 0;
        mm_dec++;
        if (mm_dec > 5) { //new hour
          mm_dec = 0;
          hh_dig++;
          if ( hh_dig > 9 || (hh_dec >= 2 && hh_dig > 3)) {
            hh_dig = 0;
            hh_dec++;
            if (hh_dec > 2) { //new day
              hh_dec = 0;
            }
          }
        }
        //resync every 10 minutes (after incrementing the other numbers)

      }
    }
  }
}

byte ucount = 0;
void dot_processing() {
  //ucount++;
  //if (ucount > 25) ucount = 0;

  if (second() % 2) {

    clockleds[POS_DOTS1  ] = setColor;
    clockleds[POS_DOTS1  ].nscale8(bfac);
  }
  else {
    //clockleds[POS_DOTS1] = CRGB::Black;
    clockleds[POS_DOTS1  ].nscale8(0xF8);
    //clockleds[POS_DOTS1] = clockleds[POS_DOTS1  ];
  }
}



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
                       0b1101101,// 24 s
                       0b1101011,// 25 y
                       0b1010001,// 26 n
                    };

void setNum(int digit, byte number) {
  //Serial.print("dig:" + String(digit) + " ");
  //if (number >= 0 && number <= 10) {
  byte mask = 0x01;
  for (int i = 0; i < 7; i++) {
    clockleds_prep[digit + i] = (mask & digit_bits[number]) ? setColor : CRGB::Black ;
    //Serial.print(clockleds_prep[digit + i], BIN);
    mask <<= 1;
  }
  //}
  //Serial.println(" number:" + String(number));
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



byte incnt = 0;

time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}


