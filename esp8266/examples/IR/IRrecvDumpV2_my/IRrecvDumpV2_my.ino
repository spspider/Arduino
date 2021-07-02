/*
   IRremoteESP8266: IRrecvDumpV2 - dump details of IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.

   Copyright 2009 Ken Shirriff, http://arcfn.com
   Copyright 2017 David Conran

   Example circuit diagram:
    https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving

   Changes:
     Version 0.3 November, 2017
       - Support for A/C decoding for some protcols.
     Version 0.2 April, 2017
       - Decode from a copy of the data so we can start capturing faster thus
         reduce the likelihood of miscaptures.
   Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
*/
#define DECODE_AC
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>
#endif  // DECODE_AC

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 14
// e.g. D5 on a NodeMCU board.
#define RECV_PIN 2

// The Serial connection baud rate.
// i.e. Status message will be sent to the PC at this baud rate.
// Try to avoid slow speeds like 9600, as you will miss messages and
// cause other problems. 115200 (or faster) is recommended.
// NOTE: Make sure you set your Serial Monitor to the same speed.
#define BAUD_RATE 115200

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
#define CAPTURE_BUFFER_SIZE 1024

// TIMEOUT is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
// This parameter is an interesting trade-off. The longer the timeout, the more
// complex a message it can capture. e.g. Some device protocols will send
// multiple message packets in quick succession, like Air Conditioner remotes.
// Air Coniditioner protocols often have a considerable gap (20-40+ms) between
// packets.
// The downside of a large timeout value is a lot of less complex protocols
// send multiple messages when the remote's button is held down. The gap between
// them is often also around 20+ms. This can result in the raw data be 2-3+
// times larger than needed as it has captured 2-3+ messages in a single
// capture. Setting a low timeout value can resolve this.
// So, choosing the best TIMEOUT value for your use particular case is
// quite nuanced. Good luck and happy hunting.
// NOTE: Don't exceed MAX_TIMEOUT_MS. Typically 130ms.
#if DECODE_AC
#define TIMEOUT 50U  // Some A/C units have gaps in their protocols of ~40ms.
// e.g. Kelvinator
// A value this large may swallow repeats of some protocols
#else  // DECODE_AC
#define TIMEOUT 15U  // Suits most messages, while not swallowing many repeats.
#endif  // DECODE_AC
// Alternatives:
// #define TIMEOUT 90U  // Suits messages with big gaps like XMP-1 & some aircon
// units, but can accidentally swallow repeated messages
// in the rawData[] output.
// #define TIMEOUT MAX_TIMEOUT_MS  // This will set it to our currently allowed
// maximum. Values this high are problematic
// because it is roughly the typical boundary
// where most messages repeat.
// e.g. It will stop decoding a message and
//   start sending it to serial at precisely
//   the time when the next message is likely
//   to be transmitted, and may miss it.

// Set the smallest sized "UNKNOWN" message packets we actually care about.
// This value helps reduce the false-positive detection rate of IR background
// noise as real messages. The chances of background IR noise getting detected
// as a message increases with the length of the TIMEOUT value. (See above)
// The downside of setting this message too large is you can miss some valid
// short messages for protocols that this library doesn't yet decode.
//
// Set higher if you get lots of random short UNKNOWN messages when nothing
// should be sending a message.
// Set lower if you are sure your setup is working, but it doesn't see messages
// from your device. (e.g. Other IR remotes work.)
// NOTE: Set this value very high to effectively turn off UNKNOWN detection.
#define MIN_UNKNOWN_SIZE 12
// ==================== end of TUNEABLE PARAMETERS ====================


// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);

decode_results results;  // Somewhere to store the results

// Display the human readable state of an A/C message if we can.
void dumpACInfo(decode_results *results) {
  String description = "";
#if DECODE_DAIKIN
  if (results->decode_type == DAIKIN) {
    IRDaikinESP ac(0);
    ac.setRaw(results->state);
    description = ac.toString();
  }
#endif  // DECODE_DAIKIN
#if DECODE_FUJITSU_AC
  if (results->decode_type == FUJITSU_AC) {
    IRFujitsuAC ac(0);
    ac.setRaw(results->state, results->bits / 8);
    description = ac.toString();
  }
#endif  // DECODE_FUJITSU_AC
#if DECODE_KELVINATOR
  if (results->decode_type == KELVINATOR) {
    IRKelvinatorAC ac(0);
    ac.setRaw(results->state);
    description = ac.toString();
  }
#endif  // DECODE_KELVINATOR
#if DECODE_TOSHIBA_AC
  if (results->decode_type == TOSHIBA_AC) {
    IRToshibaAC ac(0);
    ac.setRaw(results->state);
    description = ac.toString();
  }
#endif  // DECODE_TOSHIBA_AC
#if DECODE_MIDEA
  if (results->decode_type == MIDEA) {
    IRMideaAC ac(0);
    ac.setRaw(results->value);  // Midea uses value instead of state.
    description = ac.toString();
  }
#endif  // DECODE_MIDEA
  // If we got a human-readable description of the message, display it.
  if (description != "")  Serial.println("Mesg Desc.: " + description);
}

// The section of code run only once at start-up.
void setup() {
  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  delay(500);  // Wait a bit for the serial connection to be establised.

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
}
//включить кондер
uint16_t rawData[199] = {4464, 4318,  532, 1646,  536, 562,  532, 1648,  536, 1642,  536, 562,  532, 562,  532, 1648,  536, 562,  536, 556,  532, 1648,  536, 562,  532, 560,  532, 1648,  536, 1646,  532, 562,  536, 1648,  536, 1642,  536, 562,  532, 562,  532, 1646,  536, 1648,  532, 1648,  536, 1648,  536, 1648,  536, 556,  532, 1646,  536, 1648,  538, 556,  536, 562,  532, 562,  532, 562,  532, 566,  532, 1644,  536, 1648,  532, 1648,  536, 562,  532, 562,  532, 1648,  536, 562,  532, 562,  536, 558,  532, 560,  536, 556,  536, 1648,  532, 1646,  536, 562,  532, 1648,  536, 1642,  594, 5164,  4442, 4322,  532, 1648,  536, 562,  532, 1648,  536, 1648,  532, 562,  532, 562,  536, 1642,  536, 562,  536, 558,  532, 1648,  536, 562,  532, 562,  532, 1646,  536, 1648,  532, 560,  538, 1646,  536, 1642,  536, 558,  536, 562,  532, 1646,  536, 1642,  538, 1646,  536, 1642,  536, 1648,  536, 556,  538, 1646,  532, 1646,  538, 562,  532, 562,  532, 562,  532, 560,  532, 566,  532, 1642,  536, 1646,  536, 1642,  536, 562,  532, 562,  532, 1648,  536, 560,  532, 562,  536, 556,  536, 558,  536, 562,  532, 1646,  532, 1648,  536, 562,  532, 1648,  536, 1648,  532};  // COOLIX B29FE4

// The repeating section of the code
//
void loop() {
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
    Serial.println((results.rawlen));
    //Serial.println((results.rawbuf[0]));
    Serial.print("uint16_t rawData[] =");
    String output;
    output += "] = {";
    for (uint16_t i = 1; i < results.rawlen; i++) {
      uint32_t usecs;
      for (usecs = results.rawbuf[i] * RAWTICK;
           usecs > UINT16_MAX;
           usecs -= UINT16_MAX) {
        output += uint64ToString(UINT16_MAX);
        if (i % 2)
          output += ", 0,  ";
        else
          output += ",  0, ";
      }
      output += uint64ToString(usecs, 10);
      if (i < results.rawlen - 1)
        output += ", ";  // ',' not needed on the last one
      if (i % 2 == 0)  output += " ";  // Extra if it was even.
    }
    output += "};";
    Serial.println(output);
    Serial.println();
    Serial.println(resultToSourceCode(&results));
    Serial.println("");  // Blank line between entries
    yield();  // Feed the WDT (again)
  }
}
