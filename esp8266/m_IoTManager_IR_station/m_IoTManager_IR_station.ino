//#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> //iotmanager
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <FS.h>
//#include <ESP8266WiFiMulti.h>
#define DBG_OUTPUT_PORT Serial
#include <ArduinoJson.h>
#include <stdio.h>
#include <string.h>
#include <TimeLib.h>
ESP8266WebServer server(80);
/////////////////dht
#include "DHTesp.h"
DHTesp dht;
////////////////////
#include <ESP8266SSDP.h>
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
bool try_MQTT_access = false;

bool IOT_Manager_loop = 0;
long no_internet_timer = 0;
bool internet = false;
bool IR_loop = false;
bool w433_loop = false;
//////////////////CaptivePortalAdvanced
char softAP_ssid[32] = "ESP_ap_dev_001";
char softAP_password[32] = "12345678";
char ssid[32] = "";
char password[32] = "";
int ipport = 80;
/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
char *myHostname = "esp8266";
///////////////////////////IOT///////////
String prefix   = "/IoTmanager";     // global prefix for all topics - must be some as mobile device
String deviceID = "dev01-kitchen";   // thing ID - unique device id in our project
/////////////////////////////////////////
String mqttServerName = "m20.cloudmqtt.com";
unsigned int mqttport = 16238;
String mqttuser = "spspider";
String mqttpass = "5506487";
unsigned char type_mqtt = 1;
//////////////Email///////////
char smtp_arr[] = "mail.smtp2go.com";
unsigned int smtp_port = 2525;
String to_email_addr = "spspider@mail.ru"; // destination email address
String from_email_addr = "spspider95@smtp2go.com"; //source email address
//Use this site to encode: http://webnet77.com/cgi-bin/helpers/base-64.pl/
String emaillogin = "c3BzcGlkZXI5NUBnbWFpbC5jb20="; //username
String password_email = "NTUwNjQ4Nw=="; //password_email
char timezone = 2;               // ?????????????? ???????? GTM
//String deviceID = deviceID; // ?????? SSDP
/////////////IR
bool Page_IR_opened = false;
bool geo_enable = false;
unsigned int mqttspacing = 60;
//////////////////////////////
//String jsonConfig = "{}";
////////////TimeAlarmString/////////
const unsigned char Numbers = 6;//???????????????????? ?????????????? ?? ???????????? ????????????
const unsigned char Condition = 3;//???????????????????? ????????????
int bySignalPWM[Condition][Numbers];
//////////////ZERO-CROSS//////////////////////
bool zero_cross;
unsigned long i_micros_dimming = 0;
int freqStep = 375;//75*5 as prescalar is 16 for 80MHZ
///////////////////////////////////////
char pwm_delay_long = 10;
///////////////////////////////////////////
char trying_attempt_mqtt = 0;
char internet_pin = 255;
char internet_cycle = 255;
bool internet_pin_inv = true;
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();


  setup_FS();
  test_setup();
ir_station_file_setup();
  if (loadConfig(readCommonFiletoJson("other_setup"))) { }
  server = ESP8266WebServer (ipport);
  captive_setup();
  //Server_begin();
  //






  //?????????????????????? ?? ?????????????????? SSDP ??????????????????
  //  Serial.println("Start 3-SSDP");
  SSDP_init();
  setup_IR(true);
  //////////////////////////////////////
  setup_w433();
  //  setup_websoket();
  //  setup_rest();
  /////////////////////////////////////


}
void loop() {
  loop_FS();
  captive_loop();
  test_loop();
  loop_websocket();

  loop_IR();
  loop_w433();
  //loop_time();
  loop_alarm();
  ir_station_file_loop();
  //loop_rest();



}

