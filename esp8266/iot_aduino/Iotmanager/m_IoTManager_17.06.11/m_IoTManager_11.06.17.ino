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
ESP8266WebServer server(80);
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
bool Iot_internet_access=false;

bool IOT_Manager_loop=0;
bool buttons_loop=1;
long no_internet_timer=0;
bool internet= true;

String _ssid     = "Home"; // Для хранения SSID
String _password = "sp5506487"; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name = "Update-Time"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
///////////////////////////
String IOTserver = "m20.cloud";   // SSID AP точки доступа
unsigned int IOTport = 3128; // пароль точки доступа
String IOTclient = "spspider"; // Имя SSDP
String IOTpassword = "5506487"; // Имя SSDP
//unsigned int timezone = 3;               // часовой пояс GTM
String jsonConfig = "{}";

void setup() {
  loadConfig();
    Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
captive_setup();
setup_websoket();
setup_FS();
setup_IOTManager();//Для websocket нужен InitVar();

}
void loop(){
captive_loop();
loop_websocket();
loop_FS();
if(Iot_internet_access){  loop_IOTMAnager();}
if ((!Iot_internet_access)&&(WiFi.status() == WL_CONNECTED)&&(WiFi.getMode() == WIFI_STA)){
           if (millis() > no_internet_timer + 30000 ) {//пробовать подключится каждые30 сек
          internet=true;
          Iot_internet_access=true;
          Serial.println("Connect again");
          no_internet_timer=millis();
         }
  }




}

