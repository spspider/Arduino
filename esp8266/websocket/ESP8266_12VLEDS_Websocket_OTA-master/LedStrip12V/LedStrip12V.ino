
//#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
//#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )
#include <ESP8266WiFi.h>
#include "LedStrip12V.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>
void setup() {

  Serial.begin(115200);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  WifiConnect();
  WebSocketConnect();
  MDNSConnect();
  HTTPUpdateConnect();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WifiConnect();
    WebSocketConnect();
    MDNSConnect();
  }
  else {

    webSocket.loop();
    if (millis() - lastTimeHost > 10) {
      httpServer.handleClient();
      lastTimeHost = millis();
    }
    if (millis() - lastTimeRefresh > WAIT_RAINBOW && millis() - lastTimeRefreshRainbow > rainbowDelay && rainbowFlag) {
      lastTimeRefreshRainbow = millis();
      writeWheel(cnt++, RGB);
    }
  }
}

