#include "max6675.h"
uint8_t therm1CS  = 15;
uint8_t therm2CS  = 13;
uint8_t thermDO  = 12;
uint8_t thermCLK = 14;
MAX6675 thermocouple1(thermCLK, therm1CS, thermDO);
MAX6675 thermocouple2(thermCLK, therm2CS, thermDO);
void handle_IR_Station() {
  //String requestString = server.arg("json"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
  //Serial.println("" + requestString);
  if (!root.success()) {
    Serial.println("FAIL AJAX!!!!" + server.arg("json"));
    return;
  }
  char Topic_is = root["t"];
  int newValue = root["v"];
  Serial.println(Topic_is);
  Serial.println(newValue);
  // callback_scoket(Topic_is, newValue);
}
void ir_station_file_setup() {
  server.on("/IR_Station", handle_IR_Station);
}
void ir_station_file_loop() {
  //Serial.print("Deg C = ");
  //Serial.print(thermocouple1.readCelsius());
  //Serial.print("\t Deg C2 = ");
  //Serial.println(thermocouple2.readCelsius());
}
