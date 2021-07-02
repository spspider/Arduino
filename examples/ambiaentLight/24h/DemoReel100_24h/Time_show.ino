
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

byte digits[10][7] = {{0, 1, 1, 1, 1, 1, 1}, // Digit 0
  {0, 1, 0, 0, 0, 0, 1}, // Digit 1
  {1, 1, 1, 0, 1, 1, 0}, // Digit 2
  {1, 1, 1, 0, 0, 1, 1}, // Digit 3
  {1, 1, 0, 1, 0, 0, 1}, // Digit 4
  {1, 0, 1, 1, 0, 1, 1}, // Digit 5
  {1, 0, 1, 1, 1, 1, 1}, // Digit 6
  {0, 1, 1, 0, 0, 0, 1}, // Digit 7
  {1, 1, 1, 1, 1, 1, 1}, // Digit 8
  {1, 1, 1, 1, 0, 1, 1}
};  // Digit 9 | 2D Array for numbers on 7 segment
bool Dot = true;  //Dot state
bool DST = false; //DST state
int ledColor = 0x00FF00; // Color used (in hex)

int GetTime() {

  //RTC.read(Now);
  //time_t Now = RTC.Now();// Getting the current Time and storing it into a DateTime object
  int hour1 = hour();
  int minutes1 = minute();
  int second1 = second();
  if (second1 % 2 == 0) {
    Dot = false;
  }
  else {
    Dot = true;
  };
  //minutes++;
  return (hour1 * 100 + minutes1);
};

// Check Light sensor and set brightness accordingly


// Convert time to array needet for display
void TimeToArray() {
  int Now = GetTime();  // Get time
  int cursor = 29;

  //  Serial.print("Time is: ");Serial.println(Now);
  if (DST) {  // if DST is true then add one hour
    Now += 100;
    //   Serial.print("DST is ON, time set to : ");Serial.println(Now);
  };
  if (Dot) {
    leds[14] = ledColor;
  }
  else {
    leds[14] = 0x000000;
  };
  for (int i = 1; i <= 4; i++) {
    int digit = Now % 10; // get last digit in time
    if (i == 1) {
      //      Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");
      cursor = 22;
      for (int k = 0; k <= 6; k++) {
        //        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
          leds_time[cursor] = leds[cursor];
        }
        else if (digits[digit][k] == 0) {
          leds_time[cursor] = 0x000000;
        };
        cursor ++;
      };
      //      Serial.println();
    }
    else if (i == 2) {
      //      Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");
      cursor -= 14;
      for (int k = 0; k <= 6; k++) {
        //        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
         leds_time[cursor] = leds[cursor];
        }
        else if (digits[digit][k] == 0) {
          leds_time[cursor] = 0x000000;
        };
        cursor ++;
      };
      //      Serial.println();
    }
    else if (i == 3) {
      //      Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      cursor = 7;
      for (int k = 0; k <= 6; k++) {
        //        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
           leds_time[cursor] = leds[cursor];
        }
        else if (digits[digit][k] == 0) {
          leds_time[cursor] = 0x000000;
        };
        cursor ++;
      };
      //      Serial.println();
    }
    else if (i == 4) {
      //      Serial.print("Digit1 is : ");Serial.print(digit);Serial.print(" ");
      cursor = 0;
      for (int k = 0; k <= 6; k++) {
        //        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
          leds_time[cursor] = leds[cursor];
        }
        else if (digits[digit][k] == 0) {
          leds_time[cursor] = 0x000000;
        };
        cursor ++;
      };
      //      Serial.println();
    }
    Now /= 10;
  };
};

void loop_time()  // Main loop
{
  //TimeAdjust(); // Check to se if time is geting modified
  TimeToArray(); // Get leds array with required configuration
  FastLED.show(); // Display leds array
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus() != timeNotSet) {

  }

  //delay(1000);
}
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}
