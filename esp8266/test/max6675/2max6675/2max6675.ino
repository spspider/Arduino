#include "max6675.h"
uint8_t therm1CS  = 15;
uint8_t therm2CS  = 13;
uint8_t thermDO  = 12;
uint8_t thermCLK = 14;
MAX6675 thermocouple1(thermCLK, therm1CS, thermDO);
MAX6675 thermocouple2(thermCLK, therm2CS, thermDO);

void setup() {
  Serial.begin(115200);
  // give the MAX a little time to settle
  //delay(500);
}
void loop() {
  // basic readout test
  Serial.print("Deg C = ");
  Serial.print(thermocouple1.readCelsius());
  Serial.print("\t Deg C2 = ");
  Serial.println(thermocouple2.readCelsius());
  // delay(500);
}
