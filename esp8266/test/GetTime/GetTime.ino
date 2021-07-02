
#include <ESP8266WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <ESP8266WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println();
  // put your setup code here, to run once:
  Serial.println("Setup timer:hour:" + GetTime("08:00:00", 0) + " minuts:" + GetTime("08:00:00", 1) + " seconds:" + GetTime("08:00:00", 2));

  //String Time= Time.indexOf(":");
  String Time = "08:00:00";
  Time.indexOf(":");
  Serial.println("indexOf:");
}
String GetTime(String Time, int pos) {
  if (pos == 0) {
    int i = Time.indexOf(":");
    Time = Time.substring(0, i);
  }
  else if (pos == 1)  {
    int i = Time.indexOf(":");
    Time = Time.substring(i + 1);
    i = Time.indexOf(":");
    Time = Time.substring(0, i);
  } else if (pos == 2) {
    int i = Time.indexOf(":");
    Time = Time.substring(i + 1);
    i = Time.indexOf(":");
    Time = Time.substring(i+1, Time.length());
  }

  return Time;
}
void loop() {
  // put your main code here, to run repeatedly:

}
