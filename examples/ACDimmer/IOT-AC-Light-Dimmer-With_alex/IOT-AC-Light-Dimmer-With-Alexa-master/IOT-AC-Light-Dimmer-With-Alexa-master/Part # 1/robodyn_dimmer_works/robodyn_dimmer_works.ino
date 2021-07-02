#include "hw_timer.h"
const byte zcPin = 4;//D2
const byte pwmPin = 5;  //D1

byte fade = 1;
byte state = 1;
byte tarBrightness = 255;
byte curBrightness = 0;
byte zcState = 0; // 0 = ready; 1 = processing;
void ICACHE_RAM_ATTR zcDetectISR();
byte range_old;
byte timer_range_old;
unsigned long old_millisec;

#include <Ultrasonic.h>

Ultrasonic ultrasonic(13, 12); // (Trig PIN,Echo PIN)(D7 Trig,D6 Echo)

void setup() {
  Serial.begin(115200);
  pinMode(zcPin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  attachInterrupt(zcPin, zcDetectISR, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  hw_timer_init(NMI_SOURCE, 0);
  hw_timer_set_func(dimTimerISR);
}

void loop() {
  // put your main code here, to run repeatedly:

  if ((ultrasonic.Ranging(CM) != 51) && (!timer_range_old )) {
    range_old = ultrasonic.Ranging(CM);
    timer_range_old = 1;
    old_millisec = millis();
  }
  if ((millis() > old_millisec + 100 )) {
    if (range_old < 13) {
         Serial.println(range_old);
      tarBrightness = 255 - range_old * 21;
    }
    timer_range_old = 0;
    old_millisec = millis();
 
  }

  /*
    if ((millis() > old_millisec + 100 ) && (timer_range_old) && (ultrasonic.Ranging(CM) != 51)) {
    byte max_value;
    byte min_value;

    if (ultrasonic.Ranging(CM) > range_old) {
      max_value = ultrasonic.Ranging(CM);
      min_value = range_old;
    } else {
      max_value = range_old ;
      min_value = ultrasonic.Ranging(CM);
    }
    if ((3 > max_value - min_value) ) {
      tarBrightness = 255 - ultrasonic.Ranging(CM) * 21;
      Serial.print("range__new ");
      Serial.println(ultrasonic.Ranging(CM));
      Serial.print("range_old ");
      Serial.println(range_old);
    }
    timer_range_old = 0;
    old_millisec = millis();

    }
    /*
    if (Serial.available()) {
      int val = Serial.parseInt();
      if (val > 0) {
        tarBrightness = val;
        Serial.println(tarBrightness);
      }

    }
  */

}


void dimTimerISR() {
  if (fade == 1) {
    /*
      if (curBrightness > tarBrightness || (state == 0 && curBrightness > 0)) {
      --curBrightness;
      }
      else if (curBrightness < tarBrightness && state == 1 && curBrightness < 255) {
      ++curBrightness;
      }
    */
    curBrightness = tarBrightness;
  }
  else {
    if (state == 1) {
      curBrightness = tarBrightness;
    }
    else {
      curBrightness = 0;
    }
  }

  if (curBrightness == 0) {
    state = 0;
    digitalWrite(pwmPin, 0);
  }
  else if (curBrightness == 255) {
    state = 1;
    digitalWrite(pwmPin, 1);
  }
  else {
    digitalWrite(pwmPin, 1);
  }

  zcState = 0;
}

void ICACHE_RAM_ATTR zcDetectISR() {
  if (zcState == 0) {
    //Serial.println("ok");

    zcState = 1;

    if (curBrightness < 255 && curBrightness > 0) {
      digitalWrite(pwmPin, 0);

      int dimDelay = 30 * (255 - curBrightness) + 400;//400
      hw_timer_arm(dimDelay);

    }
  }
}
