/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(2);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */


  /* Same switch as above, but using binary code */

    //mySwitch.send("0f07832");
      //mySwitch.send("000000000001010100010001");
  //delay(1000);  
  //mySwitch.send("000000000001010100010100");
  //delay(1000);

  /* Same switch as above, but tri-state code */ 
  //mySwitch.sendTriState("00000FFF0F0F");
  //delay(1000);  
  //mySwitch.sendTriState("00000FFF0FF0");
  
        mySwitch.sendTriState("f0f0f0f0f0f0cb2b2cacccb33332cb32cb3333333332cccb332a8787878787878659596566659999965996599999999996665999543c3c3c3c3c3c32cacb2b332cccccb2ccb2ccccccccccb332cccaa1e1e1e1e1e1e196565958");
  //mySwitch.send("1001010101010100101010101010000101001010000100010100001001010010001010101000");
  delay(1000);
Serial.println("Send Complete");
}
