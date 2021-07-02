/* AC Light Dimmer - Inmojo  AC Voltage dimmer with Zero cross detection  Author: Charith Fernanado http://www.inmojo.com charith@inmojo.com  License: Released under the Creative Commons Attribution Share-Alike 3.0 License.  http://creativecommons.org/licenses/by-sa/3.0 Target:  Arduino  Attach the Zero cross pin of the module to Arduino External Interrupt pin Select the correct Interrupt # from the below table Pin    |  Interrrupt # | Arduino Platform --------------------------------------- 2      |  0            |  All 3      |  1            |  All 18     |  5            |  Arduino Mega Only 19     |  4            |  Arduino Mega Only 20     |  3            |  Arduino Mega Only 21     |  2            |  Arduino Mega Only  */

int AC_LOAD = 5;    // Output to Opto Triac pin
int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF
void ICACHE_RAM_ATTR zero_crosss_int();
void setup()
{
  pinMode(AC_LOAD, OUTPUT);        // Set the AC Load as output
  attachInterrupt(4, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
}

void ICACHE_RAM_ATTR zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation :: 50Hz-> 10ms (1/2 Cycle)
  // (10000us - 10us) / 128 = 75 (Approx)
  int dimtime = (75 * dimming);
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off
}

void loop()
{
  digitalWrite(AC_LOAD, HIGH);
  
  
  dimming = 128;
  delay(100);
  dimming = 75;
  delay(100);
  dimming = 25;
  delay(100);

}
