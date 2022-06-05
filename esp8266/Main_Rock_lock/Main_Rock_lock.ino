//4Mb 1Mb 1Mb
//1Mb 64,470
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <FS.h>
#include <ArduinoJson.h>

String serverWG;
char softAP_ssid[32] = "RockLock1";
char softAP_password[32] = "12345678";

char ssid[32] = "wifi-ssid";
char password[32] = "wifi-password";
bool loop_lock_active = false;
bool Page_IR_opened = false;

char lock_pin=2; //12


bool connectAsAp = true;
bool lock_pin_invert = 0;
char open_timer = 1;
int count_timer_lock_anyway = 10;
char w433rcv = 255;

bool station_connected;
char countdown_lock = 0;

bool register_is_on;
char str_connected_mac[32] = "";
bool compare_found_global = false;

bool admin_granted = false;
char login_Authentication[32];
char pass_Authentication[32];


////////////////////////////wg
int wgD0 = 2;//желтый провод green //d0 green,d1 white
int wgD1 = 0;//синий white



char record_card = 0;


char open_type = 0;

extern "C" {
#include <user_interface.h>
}
void setup() {
  Serial.begin(115200);
  setup_FS();
  
  if (loadConfig(readCommonFiletoJson("setup_lock"))) {}
  setup_captive();
  setup_w433();
  loop_lock_active = true;
  setup_lock();
  setup_ota();
  setup_wg();
}
void loop() {
  loop_captive();
  if (loop_lock_active) {
    loop_lock();
  }
  loop_w433();
  loop_wg();
  //loop_ota();
}
