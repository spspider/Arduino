/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.

   Version 1.0 April, 2017
   Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
   Copyright 2009 Ken Shirriff, http://arcfn.com

   An IR LED circuit *MUST* be connected to ESP8266 pin 4 (D2).

   TL;DR: The IR LED needs to be driven by a transistor for a good result.

   Suggested circuit:
       https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending

   Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
       have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
       See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
       Replace the IR LED with a normal LED if you don't have a digital camera
       when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
       for your first time. e.g. ESP-12 etc.
*/

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(15);  // An IR LED is controlled by GPIO pin 4 (D2)

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[199] = {4438, 4328,  532, 1648,  536, 558,  536, 1650,  532, 1646,  532, 562,  532, 562,  536, 1646,  532, 562,  536, 558,  532, 1648,  532, 562,  536, 558,  536, 1648,  532, 1648,  532, 562,  532, 1654,  532, 1648,  532, 562,  532, 562,  532, 1648,  536, 1648,  532, 1648,  532, 1648,  538, 1646,  538, 558,  532, 1648,  536, 1648,  532, 562,  532, 562,  532, 562,  532, 562,  538, 562,  532, 1648,  532, 1648,  532, 1652,  532, 562,  532, 562,  532, 1648,  536, 558,  538, 562,  532, 558,  536, 562,  532, 562,  532, 1648,  532, 1652,  532, 562,  532, 1648,  532, 1648,  590, 5168,  4438, 4320,  536, 1648,  532, 562,  538, 1648,  532, 1648,  532, 562,  536, 556,  538, 1648,  532, 562,  536, 558,  532, 1648,  538, 556,  536, 558,  536, 1648,  532, 1648,  532, 562,  536, 1648,  532, 1648,  532, 562,  532, 562,  536, 1648,  532, 1646,  532, 1652,  532, 1648,  532, 1652,  532, 558,  536, 1648,  532, 1648,  536, 558,  536, 558,  538, 562,  532, 562,  532, 562,  538, 1644,  536, 1648,  532, 1648,  532, 562,  532, 562,  536, 1648,  532, 562,  532, 562,  538, 556,  532, 562,  532, 562,  532, 1652,  532, 1648,  532, 562,  532, 1648,  536, 1648,  536};  // COOLIX B29FE4
uint16_t rawData[200] ={1,2234,2150,291,799,295,254,291,799,293,799,293,256,291,256,290,801,291,256,268,279,293,799,291,256,293,256,290,799,306,785,293,256,290,801,268,821,291,256,293,256,291,799,293,799,293,799,291,801,291,801,268,279,290,799,293,799,293,256,291,256,293,254,293,256,291,256,268,824,291,799,293,799,293,256,291,256,291,801,291,256,293,256,268,276,293,258,289,256,293,799,293,799,291,256,293,799,290,799,310,2570,2234,2148,293,799,293,254,293,799,293,799,290,256,293,254,293,799,293,256,268,279,291,799,306,243,291,256,293,799,291,801,291,256,291,801,268,822,293,256,291,257,292,799,293,799,291,801,291,799,306,788,266,278,306,785,293,799,293,254,293,256,291,256,293,254,293,256,268,821,293,799,293,799,291,256,293,254,293,799,290,256,293,256,268,279,293,256,304,243,291,799,293,799,293,256,290,799,293,801,268};


void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {

  Serial.println("NEC");
  irsend.sendNEC(0x20DFC03F, 32);



  delay(2000);

  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(rawData, 199, 38);  // Send a raw data capture at 38kHz.

  delay(2000);
}
