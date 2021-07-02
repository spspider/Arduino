//**************************************************************//
//  Name    : shiftOutCode, Hello World                         //
//  Author  : Carlyn Maw,Tom Igoe                               //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 2;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 3;


int inByte = 0;         // incoming serial byte


void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  //count up routine
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();

  }
  if (inByte) {
    shiftOut(dataPin, clockPin, LSBFIRST, inByte);
    inByte = 0;
  } else {
    shiftOut(dataPin, clockPin, LSBFIRST, 127);
  }
}
