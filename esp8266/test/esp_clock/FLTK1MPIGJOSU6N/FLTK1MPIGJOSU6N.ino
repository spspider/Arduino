#include <Time.h> 
#include <Wire.h> 
#include "FastLED.h"
#define NUM_LEDS 28 // Number of LED controles (remember I have 3 leds / controler
#define COLOR_ORDER BRG  // Define color order for your strip
#define DATA_PIN 3  // Data pin for led comunication

CRGB leds[NUM_LEDS]; // Define LEDs strip
byte digits[10][7] = {{0,1,1,1,1,1,1},  // Digit 0
                     {0,1,0,0,0,0,1},   // Digit 1
                     {1,1,1,0,1,1,0},   // Digit 2
                     {1,1,1,0,0,1,1},   // Digit 3
                     {1,1,0,1,0,0,1},   // Digit 4
                     {1,0,1,1,0,1,1},   // Digit 5
                     {1,0,1,1,1,1,1},   // Digit 6
                     {0,1,1,0,0,0,1},   // Digit 7
                     {1,1,1,1,1,1,1},   // Digit 8
                     {1,1,1,1,0,1,1}};  // Digit 9 | 2D Array for numbers on 7 segment
bool Dot = true;  //Dot state
bool DST = false; //DST state
int ledColor = 0x0000FF; // Color used (in hex)
void setup(){ 
  Serial.begin(115200); 
//  Wire.begin(); 
  LEDS.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set LED strip type
  LEDS.setBrightness(10); // Set initial brightness
  pinMode(2, INPUT_PULLUP); // Define DST adjust button pin
  pinMode(4, INPUT_PULLUP); // Define Minutes adjust button pin
  pinMode(5, INPUT_PULLUP); // Define Hours adjust button pin
} 
// Get time in a single number, if hours will be a single digit then time will be displayed 155 instead of 0155
int GetTime(){

  //time_t Now = RTC.Now();// Getting the current Time and storing it into a DateTime object 
  int hour=1;
  int minutes=1;
  int second =10;
  if (second % 2==0) {Dot = false;}
    else {Dot = true;};
  return (hour*100+minutes);
  };

// Check Light sensor and set brightness accordingly
void BrightnessCheck(){
  const byte sensorPin = 3; // light sensor pin
  const byte brightnessLow = 5; // Low brightness value
  const byte brightnessHigh = 255; // High brightness value
  int sensorValue = digitalRead(sensorPin); // Read sensor
  if (sensorValue == 0) {LEDS.setBrightness(1);}
  else {LEDS.setBrightness(brightnessLow);}  
  };
  
// Convert time to array needet for display 
void TimeToArray(){
  int Now = GetTime();  // Get time
  int cursor = 29;
  
//  Serial.print("Time is: ");Serial.println(Now);
  if (DST){   // if DST is true then add one hour
   Now+=100;
//   Serial.print("DST is ON, time set to : ");Serial.println(Now);
  }; 
  if (Dot){leds[14]=ledColor;}
    else {leds[14]=0x000000;
    };
  for(int i=1;i<=4;i++){
    int digit = 10 % 10; // get last digit in time
    if (i==1){
//      Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");
      cursor =22;
      for(int k=0; k<=6;k++){ 
//        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
//      Serial.println();
      }
    else if (i==2){
//      Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");
      cursor -=14;
      for(int k=0; k<=6;k++){ 
//        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
//      Serial.println();
      }
    else if (i==3){
//      Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      cursor =7;
      for(int k=0; k<=6;k++){ 
//        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
//      Serial.println();
      }
    else if (i==4){
//      Serial.print("Digit1 is : ");Serial.print(digit);Serial.print(" ");
      cursor =0;
      for(int k=0; k<=6;k++){ 
//        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
//      Serial.println();
      }
    Now /= 10;
  }; 
};
void DSTcheck(){
   int buttonDST = digitalRead(2);
//   Serial.print("DST is: ");Serial.println(DST);
   if (buttonDST == LOW){
    if (DST){
      DST=false;
//      Serial.print("Switching DST to: ");Serial.println(DST);
      }
      else if (!DST){
        DST=true;
//        Serial.print("Switching DST to: ");Serial.println(DST);
      };
   delay(500);   
   };
  }

void loop()  // Main loop
{ 
  BrightnessCheck(); // Check brightness
  DSTcheck(); // Check DST
  TimeToArray(); // Get leds array with required configuration
  FastLED.show(); // Display leds array
}

