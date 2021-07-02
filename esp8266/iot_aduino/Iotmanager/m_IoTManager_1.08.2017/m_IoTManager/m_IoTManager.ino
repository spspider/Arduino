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
#include <WebSocketsServer.h>
#include <ESP8266WiFiMulti.h>
#define DBG_OUTPUT_PORT Serial
#include <ArduinoJson.h>
#include <stdio.h>
#include <string.h>

ESP8266WebServer server(80);
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
bool try_internet_access = false;

bool IOT_Manager_loop = 0;
bool buttons_loop = 1;
long no_internet_timer = 0;
bool internet = true;
bool IR_loop = false;
bool w433_loop = false;
//////////////////CaptivePortalAdvanced
char *softAP_ssid = "ESP_ap";
char *softAP_password = "12345678";
char ssid[32] = "";
char password[32] = "";

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
//////////////Email///////////
char smtp_arr[] = "mail.smtp2go.com";
unsigned int smtp_port = 2525;
String to_email_addr = "spspider@mail.ru"; // destination email address
String from_email_addr = "spspider95@smtp2go.com"; //source email address
//base64 encoded username and password_email
//Use this site to encode: http://webnet77.com/cgi-bin/helpers/base-64.pl/
String emaillogin = "c3BzcGlkZXI5NUBnbWFpbC5jb20="; //username
String password_email = "NTUwNjQ4Nw=="; //password_email
unsigned int timezone = 3;               // часовой пояс GTM
String SSDP_Name = deviceID; // Имя SSDP
//////////////////////////////
//String jsonConfig = "{}";


void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  setup_FS();
  loadConfig();

  captive_setup();
  setup_websoket();

  setup_IOTManager();//Для websocket нужен InitVar();



  //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
  SSDP_init();
  Serial.println("Setup IR");
  //setup_IR(true);
  //////////////////////////////////////
  setup_w433();

  /////////////////////////////////////
}
void loop() {
  captive_loop();
  loop_websocket();
  loop_FS();
  loop_IR();
  loop_w433();
  //loop_time();
  loop_alarm();




}

