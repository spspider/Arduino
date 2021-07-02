/*
   TimeSerial.pde
   example code illustrating Time library set through serial port messages.

   Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
   you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
  T1357041600

   A Processing example sketch to automatically send the messages is included in the download
   On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
*/

#include <TimeLib.h>


#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

void setup()  {
  Serial.begin(115200);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  time_t now_time = 1357041111UL;
  setTime(now_time);
}

void loop() {
  if (Serial.available()) {
    processSyncMessage();
  }

  delay(1000);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  //  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  unsigned int sec;
  /*
    if (Serial.find("S")) {
      sec  = Serial.parseInt();
      //sec = 28854;
      Serial.println(getTime(1, sec) + ":" + getTime(2, sec) + ":" + getTime(3, sec));
    }
    else
  */
  if (Serial.find(TIME_HEADER)) {
    Serial.println("T");
    //pctime = 1506763398;
    //String new_time_str ="1506763398";
    //char* str = "1506763398";
    //time_t t =   ConvertCharToLong(str);
    sec  = Serial.parseInt();
    int *Time = getTime(sec);

    //setTime(t);
    //sec = 28854;
    //Serial.println(String(hour(t)) + ":" + minute(t) + ":" + second(t));
    Serial.println(String(Time[0]) + ":" +String( Time[1]) + ":" + String(Time[2]));
  }

}
unsigned long ConvertCharToLong(char *pSrc) {
  int i = 1;
  unsigned long result = (int)pSrc[0] - '0';
  while (i < strlen(pSrc)) {
    result = result * 10 + ((int)pSrc[i] - '0');
    ++i;
  }
  return result;
}

unsigned long ConvertToLong(String x) {
  unsigned long y = 0;
  for (int i = 0; i < x.length(); i++) {
    char c = x.charAt(i);
    if (c < '0' || c > '9') break;
    y *= 10;
    y += (c - '0');
  }
  return y;
}
int *getTime(unsigned int sec) {
  static int Time[3];
  //float timeSet = 0.0F;
  float hour_set_float = sec / 3600.0;
  float minutes_set_float = ((hour_set_float - (int)(hour_set_float)) * 60.0);
  float sec_set_float = ((minutes_set_float - (int)(minutes_set_float)) * 60.0);
  Time[0] = round(hour_set_float);
  Time[1] = round(minutes_set_float);
  Time[2] = round(sec_set_float);

  return Time;
}
time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}

