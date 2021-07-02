//NodeMCU/ESP8266 implement WebSocketsServer to control RGB LED
//arduino-er.blogspot.com

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
//#include <ArduinoJson.h>
#include <GDBStub.h>
#include "FS.h"
#include "ExampleParser.h"
#include "JsonListener.h"

#define LED_RED     15
#define LED_GREEN   12
#define LED_BLUE    13

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String html_home;
uint8_t            socketNumber;
String text;
//ArduinoJson::StaticJsonBuffer<200> jsonBuffer;

const char *rString,*gString,*bString;
const char* orderID;
const char *ssid = "Home";
const char *password = "sp5506487";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            socketNumber = num;
            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
                //////////////////////////////////
                text = String((char *) &payload[0]);
                //String text="'{hotel:one}'";
                //DynamicJsonBuffer jsonBuffer;
                //JsonObject& json = jsonBuffer.parseObject(text); 
                //orderID = json["orderID"];
                //const char* contents = json["contents"][1]["productID"];  
                void ExampleListener::value(String value) { 
                Serial.println("orderID: " + value); 
                } 



                Serial.println(orderID);
                
                ///////////////////////////////////

            break;
    }

}

void prepareFile(){
  
  Serial.println("Prepare file system");
  SPIFFS.begin();
  
  File file = SPIFFS.open("/home.html", "r");
  if (!file) {
    Serial.println("file open failed");  
  } else{
    Serial.println("file open success");

    html_home = "";
    while (file.available()) {
      //Serial.write(file.read());
      String line = file.readStringUntil('\n');
      html_home += line + "\n";
    }
    file.close();

    //Serial.print(html_home);
  }
}

void setup() {
    ESP.eraseConfig();
    //Serial.begin(921600);
    Serial.begin(115200);

    //Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();


    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    digitalWrite(LED_RED, 1);
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 1);

      prepareFile();


         
    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    } 
    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // handle index
    server.on("/", []() {
        // send home.html
        server.send(200, "text/html", html_home);
    });
    
    if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
    }
    server.begin();


    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);

    Serial.printf("Server Start\n");

}

void loop() {
    static unsigned long l = 0;                     // only initialized once
    unsigned long t;                                // local var: type declaration at compile time



    
    t = millis();

    if((t - l) > 10000) {                            // update temp every 5 seconds
        String orderID_str= String(orderID);
        webSocket.sendTXT(socketNumber,orderID_str);
        l = t;                                      // typical runtime this IF{} == 300uS - 776uS measured
        Serial.println("websocket sended");
        yield();
    }

    webSocket.loop();
    server.handleClient();
    
}

