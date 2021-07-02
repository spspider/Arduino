/*
   IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Copyright 2009 Ken Shirriff, http://arcfn.com
   Example circuit diagram:
    https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
   Changes:
     Version 0.2 June, 2017
       Changed GPIO pin to the same as other examples.
       Used our own method for printing a uint64_t.
       Changed the baud rate to 115200.
     Version 0.1 Sept, 2015
       Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
*/

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
uint16_t RECV_PIN = 2;
uint64_t buff1 = 0;
uint64_t overflow = -1;
IRrecv irrecv(RECV_PIN);
unsigned long millis_end = 100, millis_buffer;
decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    String codeIR;
    if ((results.value != overflow) && (buff1 != 0)) {
      char charBuff1[21];
      sprintf(charBuff1, "%X", buff1);
      if (buff1 != results.value) {
        char charBuff[21];
        sprintf(charBuff, "%X", results.value);
        Serial.println(charBuff1);
        Serial.println(charBuff);
        codeIR = String(charBuff1) + String(charBuff);
      }
      else {
        Serial.print("same code");
        Serial.println(charBuff1);
        codeIR = String(charBuff1);
        buff1 = 0;
        irrecv.resume();
      }
      buff1 = 0;
    }
    if (buff1 == 0) {
      buff1 = results.value;
      irrecv.resume();
    }
    if (results.value == overflow) {
      Serial.print("overflow");
      char charBuff1[21];
      sprintf(charBuff1, "%X", buff1);
      buff1 = 0;
      codeIR = String(charBuff1);
      //Serial.println(charBuff1);
    }


    /*
      if (millis() - millis_buffer > millis_end) {
      char charBuff1[21];
      sprintf(charBuff1, "%X", buff1);
      Serial.println(charBuff1);
      Serial.println("overflow");
      buff1 = 0;
      }
    */
    //Serial.print("Str:");

    //char buff[21];
    // copy to buffer
    //sprintf(buff, "%X", results.value);
    //Serial.println(buff);
    /*
      Serial.println();
      //Serial.println(str);
      Serial.println("decode_type:" + String(results.decode_type));
      Serial.println("bits:" + String(results.bits));
      Serial.println("repeat:" + String(results.repeat));
      Serial.println("rawlen:" + String(results.rawlen));
      Serial.println("address:" + String(results.address));
      Serial.println(results.command);
      Serial.println();
    */
    irrecv.resume();  // Receive the next value
  }
  //delay(100);
}
