/* AC Light Dimmer - Inmojo  AC Voltage dimmer with Zero cross detection  Author: Charith Fernanado <a href="http://www.inmojo.com" title="http://www.inmojo.com" rel="nofollow">http://www.inmojo.com</a> <a href="mailto:charith@inmojo.com">charith@inmojo.com</a>  License: Released under the Creative Commons Attribution Share-Alike 3.0 License.  <a href="http://creativecommons.org/licenses/by-sa/3.0" title="http://creativecommons.org/licenses/by-sa/3.0" rel="nofollow">http://creativecommons.org/licenses/by-sa/3.0</a> Target:  Arduino  Attach the Zero cross pin of the module to Arduino External Interrupt pin Select the correct Interrupt # from the below table Pin    |  Interrrupt # | Arduino Platform --------------------------------------- 2      |  0            |  All 3      |  1            |  All 18     |  5            |  Arduino Mega Only 19     |  4            |  Arduino Mega Only 20     |  3            |  Arduino Mega Only 21     |  2            |  Arduino Mega Only  */

int AC_LOAD = 3;    // Output to Opto Triac pin
int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF
void ICACHE_RAM_ATTR zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation :: 50Hz-> 10ms (1/2 Cycle)
  // (10000us - 10us) / 128 = 75 (Approx)
  int dimtime = (75*dimming);      
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off
}
void setup()
{
  pinMode(AC_LOAD, OUTPUT);        // Set the AC Load as output
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
}



void loop()
{
  dimming = 128; 
  delay(100);
  dimming = 75;  
  delay(100);
  dimming = 25;  
  delay(100);

}
