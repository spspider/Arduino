#include <Adafruit_GFX.h>
#include <gfxfont.h>

//#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> //iotmanager
#include <EEPROM.h>
//#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>     //Local WebServer used to 
//////////////////////////////////compass
//#include <AS5600.h>
//AS5600 encoder;
////////////////////////////////////////////
//serve the configuration portal
#include <FS.h>
//#include <ESP8266WiFiMulti.h>
#define DBG_OUTPUT_PORT Serial

//Encoder//Serial
bool EncoderA_prev = false;
unsigned char EncoderIA = 255;
unsigned char EncoderIB = 255;
unsigned char engineA = 255;
unsigned char engineB = 255;
unsigned char speed_Enc = 255;

void ICACHE_RAM_ATTR doEncoderA();
void ICACHE_RAM_ATTR doEncoderB();


//////////compass/////////////
//#include <QMC5883L.h>
//#include <Wire.h>
//#include <rBase64.h>
#include <MD5.h>


//QMC5883L compass;

//ws8211//////////////////////////////
//void loadLimits();
//void setup_ws2811();
//bool LoadData_set_leds(char json[400]);
//bool LoadData(char json[200]);
//void one_sec();
//void loop_ws2811();

///////////////////////////////

#include <ArduinoJson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TimeLib.h>
ESP8266WebServer server(80);
//ESP8266WebServer server = new ESP8266Webserver(80);
/////////////////dht
extern "C" {
#include <user_interface.h>
}

#include "DHTesp.h"
DHTesp dht;

////////////////////
///emon//
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
float PowerCorrection = 111.1;
/////////
#include <ESP8266SSDP.h>

///////////////////////////////////Dimmer
//#include <RBDdimmer.h>//

//#define USE_SERIAL  SerialUSB //Serial for boards whith USB serial port

//#define outputPinDimmer  16//D0
//#define zerocrossDimmer  5//D1 // for boards with CHANGEBLE input pins
//dimmerLamp dimmer(outputPinDimmer, zerocrossDimmer); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
//uint8_t DimmerVal = 0;
/////////////////////////////////////////



/////Ultrasonic///////////////
//////////////////////////////
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
bool try_MQTT_access = false;

bool IOT_Manager_loop = 0;
int no_internet_timer = 0;
bool internet = false;
//////////////////CaptivePortalAdvanced
char softAP_ssid[32] = "ESP_ap_dev_001";
char softAP_password[32] = "12345678";
char ssid[32] = "";
char password[32] = "";
short unsigned int ipport = 80;
/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";
///////////////////////////IOT///////////
char prefix[20]  = "/IoTmanager";     // global prefix for all topics - must be some as mobile device
char deviceID[20] = "dev01-kitchen";   // thing ID - unique device id in our project
/////////////////////////////////////////
char mqttServerName[25] = "m20.cloudmqtt.com";
unsigned int mqttport = 16238;
//String mqttuser = "spspider";
char mqttuser[15] = "spspider";
//String mqttpass = "5506487";
char mqttpass[15] = "5506487";
unsigned char type_mqtt = 1;
//////////////Email///////////
char smtp_arr[] = "mail.smtp2go.com";
short unsigned int smtp_port = 2525;
String to_email_addr = "spspider@mail.ru"; // destination email address
String from_email_addr = "spspider95@smtp2go.com"; //source email address
//Use this site to encode: http://webnet77.com/cgi-bin/helpers/base-64.pl/
String emaillogin = "c3BzcGlkZXI5NUBnbWFpbC5jb20="; //username
String password_email = "NTUwNjQ4Nw=="; //password_email
char timezone = 2;               // часовой пояс GTM
//String deviceID = deviceID; // Имя SSDP
/////////////IR
bool Page_IR_opened = false;
bool geo_enable = false;
bool loop_433 = false;
bool ir_loop = false;
bool wifi_scan = true;
bool ws8211_loop = true;
bool save_stat = false;






bool loop_alarm_active = true;
bool check_internet = true;
short unsigned int mqttspacing = 60;
///////////////////433
char w433rcv = 255;
uint8_t w433send = 255;
//////////////////////////////
//String jsonConfig = "{}";
////////////TimeAlarmString/////////
const unsigned char Numbers = 1;//количество условий в каждой кнопке
const unsigned char Condition = 1;//количество кнопок
unsigned char save_stat_long = 0;                     // only initialized once
short int bySignalPWM[Condition][Numbers];
///////////////////////////////////////
uint8_t pwm_delay_long = 10;
///////////////////////////////////////////
//char trying_attempt_mqtt = 0;
uint8_t router = 255;
//замок:
unsigned char countdown_lock = 0;
unsigned int onesec;
unsigned long millis_strart_one_sec;
//uint8_t onesec_240;
//uint8_t check_my_alarm;
uint8_t onesec_255;

bool license = 0;
//bool get_new_pin_value_ = true;

bool test_action = false;
//bool internet_pin_inv=0;
//char internet_cycle = 255;

unsigned char PWM_frequency = 1;
void setup() {
  //Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  //getEEPROM_char(0) == 13 ? license = 1 : license = 0;
  setup_FS();
  //test_setup();
  //analogWriteFreq(150);
  MD5Builder md5;
  md5.begin();
  md5.add(WiFi.macAddress() + "password");
  md5.calculate();
  if (readCommonFiletoJson("activation") == md5.toString()) {
    license = 1;
  }

  //char* testString="123";
  //short int testInt = atoi(testString);
  //Serial.print("!!!!!!!!!!!!!!!!!!!!!testInt:");
  //Serial.println(testInt);
  if (loadConfig(readCommonFiletoJson("other_setup"))) { }
  //server = ESP8266WebServer (ipport);


  captive_setup();
  //setup_ws2811();//include ws2811.in
  setup_WOL();

  //  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  setup_compass();
  //Настраиваем и запускаем SSDP интерфейс
  //  Serial.println("Start 3-SSDP");
  //SSDP_init();
  // setup_IR(true);
  //////////////////////////////////////
  // setup_w433();
  //setup_wg();
  /////////////////////////////////////
  callback_from_stat();
}
void loop() {
  captive_loop();
  test_loop();
  loop_websocket();
  if (ir_loop) {
    //loop_IR();
  }
  if (w433rcv != 255) {
    //loop_w433();
  }
  if (ws8211_loop == true) {
    //loop_ws2811();//include ws2811.in
  }

  if (loop_alarm_active) {
    loop_alarm();
  }

  if (millis() > 1000L + millis_strart_one_sec) {
    onesec++;
    //onesec_255 = onesec_255 > 240 ?  0; check_my_alarm = 0 : onesec_255 + 1;
    //  if (onesec_240 > 240) {
    //   onesec_240 = 0;
    //   check_my_alarm = 0;
    //   Serial.println("onesec_240=0");
    // }
    //  Serial.println(onesec_240);
    //onesec_240++;
    onesec_255++;
    check_if_there_next_times();
    //Serial.println(onesec_255);
    //one_sec();//include ws2811.in
    one_sec_lock();
    millis_strart_one_sec = millis();
  }
  loop_ota();
  EncoderCalc();
  //ultrasonic_loop();

  //loop_wg();
}

