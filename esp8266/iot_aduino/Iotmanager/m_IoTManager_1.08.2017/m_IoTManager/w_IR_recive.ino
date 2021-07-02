/*
   IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 Sept, 2015
   Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
*/
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

uint16_t RECV_PIN = 2;
uint64_t buff1 = 0;
uint64_t overflow = -1;
IRrecv irrecv(RECV_PIN);

decode_results results;

unsigned long last_millis, wait_millisec = 30000;

void setup_IR(bool isOn)
{
  if (isOn) {
    Serial.println("Setup IR");
    irrecv.enableIRIn(); // Start the receiver
  }
  else if (!isOn) {
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Disable IR!!!!!!!!!!!!!!!!!!!");
    //irrecv.disableIRIn();
    //ESP.restart();
  }
}

void loop_IR() {

  if (!IR_loop) {//если выключен IR, то
    //last_millis = millis();//обновляем последнее время
  }
  // if (WebSocketConnected) {
  else if (IR_loop) {
    if (irrecv.decode(&results)) {
      String codeIR;
      if ((results.value != overflow) && (buff1 != 0)) {
        char charBuff1[21];
        sprintf(charBuff1, "%X", buff1);
        if (buff1 != results.value) {
          char charBuff[21];
          sprintf(charBuff, "%X", results.value);
          //Serial.println(charBuff1);
          //Serial.println(charBuff);
          codeIR = String(charBuff1) + String(charBuff);
          server.send(200, "text/plain", codeIR);
        }
        else {
          //Serial.print("same code");
          //Serial.println(charBuff1);
          codeIR = String(charBuff1);
          buff1 = 0;
          irrecv.resume();
          server.send(200, "text/plain", codeIR);
        }
        buff1 = 0;
      }
      if (buff1 == 0) {
        buff1 = results.value;
        irrecv.resume();
      }
      if (results.value == overflow) {
        //Serial.print("overflow");
        char charBuff1[21];
        sprintf(charBuff1, "%X", buff1);
        buff1 = 0;
        codeIR = String(charBuff1);
        server.send(200, "text/plain", codeIR);
        //Serial.println(charBuff1);
      }

      irrecv.resume(); // Receive the next value IRRecieve

      //IR_loop = false;
      //Serial.println("выключение IR");
      //irrecv.disableIRIn();
      //server.send(200, "text/json", root);


    }
    //delay(100);
    /*
      if (millis() - last_millis > wait_millisec) { //сейчас-последнее время> усановленного
      last_millis = millis();//обнуляем

      server.send(200, "text/plain", "IRRecieve=TimeLimit");//отправляем завершающую строку
      setup_IR(false);
      //irrecv.disableIRIn();
      //irrecv.resume();
      Serial.println("ошибка принятия IR");
      IR_loop = false;
      }
    */
  }
  //}


}
