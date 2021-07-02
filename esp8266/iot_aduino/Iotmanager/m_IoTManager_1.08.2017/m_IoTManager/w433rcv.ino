/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup_w433() {
  //Serial.begin(9600);
  //mySwitch.enableReceive(15);  // Receiver on interrupt 0 => that is pin #2
  //mySwitch.disableReceive(15);
}

void loop_w433() {

    if (w433_loop) {
      if (mySwitch.available()) {

        int value = mySwitch.getReceivedValue();

        if (value == 0) {
          Serial.print("Unknown encoding");
        } else {
          Serial.print("Received ");
          Serial.print( mySwitch.getReceivedValue() );
          Serial.print(" / ");
          Serial.print( mySwitch.getReceivedBitlength() );
          Serial.print("bit ");
          Serial.print("Protocol: ");
          Serial.println( mySwitch.getReceivedProtocol() );
          server.send(200, "text/plain", String(value));
        }

        mySwitch.resetAvailable();
      }
      //mySwitch.resetAvailable();
    }
    else if (!w433_loop) {
    }
  
}
