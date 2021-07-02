/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 Sept, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>

int RECV_PIN = 2; //an IR detector/demodulatord is connected to GPIO pin 2

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup_IR()
{

  irrecv.enableIRIn(); // Start the receiver
}

void loop_IR() {
  if (WebSocketConnected){
      if (IR_loop){
        if (irrecv.decode(&results)) {
          Serial.print("IrCode:");
          String stringIR =  String(results.value, HEX);
      
          Serial.println(stringIR);
          bool success;
          String buffer = "/" + stringIR;
          success = webSocket.sendTXT(NumberWebSocket,buffer);
          irrecv.resume(); // Receive the next value
        }
        delay(100);
      }
  }
}
