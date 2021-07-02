/*
  Reading a serial ASCII-encoded string.

  This sketch demonstrates the Serial parseInt() function.
  It looks for an ASCII string of comma-separated values.
  It parses them into ints, and uses those to fade an RGB LED.

  Circuit: Common-Cathode RGB LED wired like so:
   Red anode: digital pin 3
   Green anode: digital pin 5
   Blue anode: digital pin 6
   Cathode : GND

  created 13 Apr 2012
  by Tom Igoe

  modified 14 Mar 2016
  by Arturo Guadalupi

  This example code is in the public domain.
*/

// pins for the LEDs:

#include <ArduinoJson.h>
#include <stdio.h>
#include <string.h>

const unsigned int Numbers = 5;
int tID_a[Numbers];
int type_a[Numbers];
unsigned int timer_a[Numbers];
char timerType_a[Numbers][20];//надо переделать в int
int act_a[Numbers];
char actBtn_a[Numbers][20];
char actOn_a[Numbers][10];//надо переделать в int



void setup() {
  Serial.begin(115200);
  char json[] =  "{\"tID\": \"[ 0 ]\", \"type\": \"[ 3 ]\", \"timer\": [ 10 ], \"timerType\": [ \"сек\" ], \"act\": [ 0 ], \"actBtn\": [ \"Light-1\" ], \"actOn\": [ \"вкл\" ], \"ID\": \"null\", \"Numbers\": \"0\"}";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(json);

  const char *timerType = rootjs["timerType"][0];

  // char* в char[6]
  char *s = "abcdef";
  char c[6];
  strncpy(c, s, sizeof(c) - 1);
  // strncpy is not adding a \0 at the end of the string after copying it so you need to add it by yourself
  c[sizeof(c) - 1] = '\0';
  Serial.println(c);



  const char *timerType_buff = rootjs["timerType"][0];
  //strncpy(timerType_char, timerType_buff, sizeof(timerType_char) - 1);
  //timerType_char[sizeof(timerType_char) - 1] = '\0';

  char timerType_char[20];
  Serial.print("const char* в char[20]");
  strncpy(timerType_char, timerType, sizeof(timerType_char) - 1);
  timerType_char[sizeof(timerType_char) - 1] = '\0';
  Serial.println(timerType_char);
  //

  snprintf(timerType_a[0], sizeof timerType_a[0], "%s", timerType_buff);//delete* timerType_buff;

  Serial.print("const char* в char*");
  //const char* в char*
  char *buffer = (char*)timerType;
  Serial.println(buffer);

  ///////////////
  String prefix = "{\"tID\": \"[ 0 ]\", \"type\": \"[ 3 ]\", \"timer\": [ 10 ], \"timerType\": [ \"сек\" ], \"act\": [ 0 ], \"actBtn\": [ \"Light-1\" ], \"actOn\": [ \"вкл\" ], \"ID\": \"null\", \"Numbers\": \"0\"}";
  Serial.println("///////////////////");
  char __prefix[prefix.length()+1];
  prefix.toCharArray(__prefix, prefix.length()+1);
  Serial.println("///////////////////");
  Serial.println(__prefix);
  //////////////////////


//  String sTopic_s   = prefix + "/" + deviceID + "/" + String( descr[i]) + String(id[i]);
//  strncpy ( sTopic[i], sTopic_s, sizeof(sTopic_s) );

  timerType_a[0][sizeof(timerType_a[0]) - 1] = '\0';
  //timerType_a[0]=timerType_char;
  Serial.println(timerType_char);
  Serial.println(timerType_a[0]);
}
void loop()
{
  /*
    byte inChar;

    inChar = Serial.read();

    if(inChar == 'e')
    {

    }
  */
}







