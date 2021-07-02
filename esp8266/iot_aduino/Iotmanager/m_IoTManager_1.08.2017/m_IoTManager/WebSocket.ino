/*
   WebSocketServer.ino

    Created on: 22.05.2015

*/
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
WebSocketsServer webSocket = WebSocketsServer(81);

//unsigned char NumberWebSocket;
#define USE_SERIAL Serial1
uint8_t NumberWebSocket;
bool WebSocketConnected = false;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  NumberWebSocket = num;
  switch (type) {
    case WStype_DISCONNECTED:
      WebSocketConnected = false;
      USE_SERIAL.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        WebSocketConnected = true;
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

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

    for (int i = 0; i < nWidgets; i++) {
      if (pinmode[i] == "low_pwm") {
        newtimePWM = millis();//таймер для 1 сек
        if (!low_pwm_off) {
          if (newtimePWM - oldtimePWM > (low_pwm[i] * 10)) { // если миллисекунды с старта больше записанного значения
            digitalWrite(pin[i], 0);//далее - выключаем
            float sec_pass = ((newtimePWM - oldtimePWM) * 1.0 / 1000.0);
            //Serial.print("pin:" + String(pin[i]) + " off ");
            //Serial.println(sec_pass, 1);
            low_pwm_off = true;
          }
        }
        if (newtimePWM - oldtimePWM > 10240) { // 1 sec//
          digitalWrite(pin[i], 1);//включаем снова
          //oldtimePWM = newtimePWM; //начинаем цикл заново
          float sec_pass = ((newtimePWM - oldtimePWM) * 1.0 / 1000.0);
          //Serial.print("pin:" + String(pin[i]) + " on ");
          //Serial.println(sec_pass, 1);
          oldtimePWM = newtimePWM;
          low_pwm_off = false;
        }
      }
    }


    static unsigned long l = 0;                     // only initialized once
    unsigned long t;
    webSocket.loop();

    t = millis();

    if ((t - l) > 10000) {                           // update temp every 5 seconds
      //String orderID_str= String(orderID);

      //webSocket.sendTXT(numSocket,root);
      l = t;                                      // typical runtime this IF{} == 300uS - 776uS measured
      //Serial.println("websocket sended");
      yield();
    }

  }

