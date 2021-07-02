/* Create a WiFi access point and provide a web server on it so show temperature. 
   Originally published by markingle on http://www.esp8266.com/viewtopic.php?p=47535
   Refactored and enhanced for Hackster.io by: M. Ray Burnette 20160620
   Arduino 1.6.9 on Linux Mint 64-bit version 17.3 compiled: 20160706 by Ray Burnette
    Sketch uses 284,865 bytes (27%) of program storage space. Maximum is 1,044,464 bytes.
    Global variables use 38,116 bytes (46%) of dynamic memory, leaving 43,836 bytes for local variables. Maximum is 81,920 bytes.
*/

#include <FS.h>
#include <WebSocketsServer.h>
#include "HelperFunctions.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Hash.h>
#include <ESP8266mDNS.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  SPIFFS.begin();
  Serial.println(); Serial.print("Configuring access point...");
  
  WiFi.begin ( "Home", "sp5506487");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
    
      if(MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
//    Serial.print("AP IP address: "); Serial.println(myIP);

  server.on("/", HTTP_GET, []() {
    handleFileRead("/");
  });

  server.onNotFound([]() {                          // Handle when user requests a file that does not exist
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  webSocket.begin();                                // start webSocket server
  webSocket.onEvent(webSocketEvent);                // callback function

  server.begin();
      MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
  Serial.println("HTTP server started");
  yield();
}

void loop() {
    static unsigned long l = 0;                     // only initialized once
    unsigned long t;                                // local var: type declaration at compile time
    
    t = millis();

    if((t - l) > 5000) {                            // update temp every 5 seconds
        analogSample(); yield();
        webSocket.sendTXT(socketNumber, "wpMeter,Arduino," + temp_str + ",1");
        l = t;                                      // typical runtime this IF{} == 300uS - 776uS measured
        Serial.print( ADCvalue);Serial.println(": " + temp_str + "F");
        yield();
    }

    server.handleClient();
    webSocket.loop();
}

