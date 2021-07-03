/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
//#include <PubConfig_websocket.ino>
ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);

//unsigned char NumberWebSocket;
#define USE_SERIAL Serial1
uint8_t NumberWebSocket;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
NumberWebSocket=num;
    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
        
            }
            break;
        case WStype_TEXT:
            char* text = ((char *) &payload[0]);
            WebScoketCallback(text);


        String text2 = String((char *) &payload[0]);
        if (text2 == "HELLO") {
            Serial.println("HELLO recieved");
            //json_create_text();
            pubConfigWS();
        }
        

            break;
            /*
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
            */
    }

}

void setup_websoket() {


    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop_websocket() {
    static unsigned long l = 0;                     // only initialized once
    unsigned long t;  
    webSocket.loop();
    
        t = millis();

    if((t - l) > 10000) {                            // update temp every 5 seconds
        //String orderID_str= String(orderID);
        
        //webSocket.sendTXT(numSocket,root);
        l = t;                                      // typical runtime this IF{} == 300uS - 776uS measured
        Serial.println("websocket sended");
        yield();
    }
    
}
