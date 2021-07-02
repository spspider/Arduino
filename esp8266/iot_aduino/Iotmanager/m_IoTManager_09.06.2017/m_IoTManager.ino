#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
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
ESP8266WebServer server(80);
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
bool Iot_internet_access=false;

bool IOT_Manager_loop=0;
bool buttons_loop=1;

void setup() {
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





}

