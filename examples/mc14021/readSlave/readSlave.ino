/*
int latchPin = 2;//p/s
int dataPin = 4;//q8
int clockPin = 3;//clock
int dataPinOut = 8;//q8
*/
int latchPin = 15;//p/s//d8
int dataPin = 13;//q8//d7
int clockPin = 14;//clock/d5
int dataPinOut =12;//q8/6

int ReadBytes;
int inByte = 0;         // incoming serial byte

boolean Strobe, nbytesLow;
int npulse;

unsigned long Data_Shiftreg;
void ICACHE_RAM_ATTR Strobe_In()
{
  Strobe = 1;
  //Serial.println(npulse, DEC);
  // npulse = 0;
  //  detachInterrupt(1);  //stop ck interrupt
  //if (Data_Shiftreg==127) {
  //Serial.print("n: ");
  // Serial.println(npulse);
  //Serial.println(Data_Shiftreg, BIN);
  //  }


}


void ICACHE_RAM_ATTR Ck_In()
{
  // if (npulse < 7) {

  // a = digitalRead(dataPin);
  /*
    if (digitalRead(dataPin)) {
    //set the bit to 0 no matter what
    Data_Shiftreg = Data_Shiftreg | (1 << npulse);
    }
    else {
    //turn it off -- only necessary for debuging
    //print statement since myDataIn starts as 0
    // pinState = 0;
    }
    //*/

  if (nbytesLow) {
    Data_Shiftreg = 0;
    npulse = 0;
    Strobe = 0;
    ReadBytes = 0;
    nbytesLow = 0;
  }

  ///*
  if (npulse > 7) {
    // detachInterrupt(digitalPinToInterrupt(clockPin));  //stop ck interrupt
    // if (Data_Shiftreg != 127) {
    Serial.println(Data_Shiftreg, BIN);
    // }
    ReadBytes = Data_Shiftreg;
    Data_Shiftreg = 0;
    npulse = 0;
    Strobe = 0;
  }
  // */


  boolean a = digitalRead(dataPin);
  Data_Shiftreg = Data_Shiftreg | a;
  Data_Shiftreg = Data_Shiftreg << 1UL;
  //Data_Shiftreg = Data_Shiftreg | (1 << a);

  npulse++;

}
void setup()
{
  pinMode(latchPin, INPUT);
  pinMode(clockPin, INPUT);
  pinMode(dataPin, INPUT);
  pinMode(dataPinOut, OUTPUT);
  Serial.begin(115200);
  Serial.println("Test");
  attachInterrupt(digitalPinToInterrupt(latchPin), Strobe_In, HIGH); // latchPin interrupt on High.
  attachInterrupt(digitalPinToInterrupt(clockPin), Ck_In, HIGH); // CLOCK interrupt on High.
}
ICACHE_RAM_ATTR void myInterruptFunction() {
  //some code here
}
void loop()
{
  readSerial();
  if (Strobe) {
    //  detachInterrupt(digitalPinToInterrupt(clockPin));  //stop ck interrupt
    //attachInterrupt(0, Strobe_In, LOW); // STROBE interrupt on High.
    //if(npulse==6){
    //shiftOut(dataPinOut, clockPin, LSBFIRST, ReadBytes);


    //  }
    if (npulse < 7) {
      // Serial.println("ERROOR");
      nbytesLow = 1;
    }
    // delayMicroseconds(1000);
    Data_Shiftreg = 0;
    npulse = 0;
    Strobe = 0;


  }

}
void readSerial() {
  if (Serial.available() > 0) {
    // get incoming byte:
    //inByte = Serial.read();
    inByte = Serial.parseInt();
    if (inByte) {
      //Serial.println(inByte, DEC);
      //shiftOut(dataPinOut, clockPin, LSBFIRST, inByte);
      inByte = 0;
    }
  }
}

