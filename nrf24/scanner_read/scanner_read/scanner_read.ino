/*
  Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

/**
   Channel scanner

   Example to detect interference on the various channels available.
   This is a good diagnostic tool to check whether you're picking a
   good channel for your application.

   Inspired by cpixip.
   See http://arduino.cc/forum/index.php/topic,54795.0.html
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

struct dataStruct {
  unsigned long _micros;
  float value;
} myData;
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8

RF24 radio(7, 8);

//
// Channel info
//

const uint8_t num_channels = 126;
uint8_t values[num_channels];

//
// Setup
//

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(115200);
  printf_begin();
  Serial.println(F("\n\rRF24/examples/scanner/"));

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setAutoAck(false);

  // Get into standby mode
  radio.startListening();
  radio.stopListening();

  radio.printDetails();

  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x", i >> 4);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    printf("%x", i & 0xf);
    ++i;
  }
  Serial.println();
}

//
// Loop
//

const int num_reps = 100;

void loop(void)
{
  // Clear measurement values
  memset(values, 0, sizeof(values));

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // Select this channel
      radio.setChannel(i);


      radio.startListening();
      if ( radio.available()) {
        Serial.println("ch:" + String(i));
        // Variable for the received timestamp
        while (radio.available()) {                          // While there is data ready
          radio.read( &myData, sizeof(myData) );             // Get the payload
        }

        delayMicroseconds(128);
        radio.stopListening();                               // First, stop listening so we can talk
        myData.value += 0.01;                                // Increment the float value
        radio.write( &myData, sizeof(myData) );              // Send the final one back.
        // Now, resume listening so we catch the next packets.
        Serial.print(F("Sent response "));
        Serial.print(myData._micros);
        Serial.print(F(" : "));
        Serial.println(myData.value);
      }


      // Did we get a carrier?
      if ( radio.testCarrier() ) {
        ++values[i];
      }
    }
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x", min(0xf, values[i]));
    ++i;
  }
  Serial.println();
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
