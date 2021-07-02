// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!



uint32_t recieve_red;
uint32_t recieve_green;
uint32_t recieve_blue;



void WebScoketCallback(char* text){
  DynamicJsonBuffer jsonBufferParse;
  JsonObject& jsonParse = jsonBufferParse.parseObject(text);
  
  // Test if parsing succeeds.
  if (!jsonParse.success()) {
  Serial.println("parseObject() failed");
  return;
  }
  const char* Topic_is = jsonParse["topic"];
  const char* newValue = jsonParse["newValue"];
  //Serial.print(Topic_is);
  //Serial.print(newValue);
  callback_scoket(Topic_is,newValue);
  }
  void callback_scoket(String Topic_is,String payload_is){

  Serial.print("Get data from subscribed topic ");
  Serial.print(Topic_is);
  Serial.print(" => ");
  Serial.println(payload_is);

  if (Topic_is == sTopic[0] + "/control") {
    if (payload_is == "0") {
       newValue = 0; // inverted
       stat[0] = stat0;
    } else {
       newValue = 1;
       stat[0] = stat1;
    }
    digitalWrite(pin[0],newValue);
    pubStatusWS(sTopic[0], stat[0],true);
   
 } else if (Topic_is == sTopic[1] + "/control") {
    if (payload_is == "0") {
       newValue = 0; // inverted
       stat[1] = stat0;
    } else {
       newValue = 1; // inverted
       stat[1] = stat1;
    }
    digitalWrite(pin[1],newValue);
    pubStatusWS(sTopic[1], stat[1],true);
 } else if (Topic_is == sTopic[2] + "/control") {
    String x = payload_is;
    analogWrite(pin[2],1023-x.toInt());
    stat[2] = setStatus(x);
    pubStatusWS(sTopic[2], stat[2],true);
 } else if (Topic_is == sTopic[3] + "/control") {
   // ADC : nothing, display only
 } else if (Topic_is == sTopic[4] + "/control") {
    if (payload_is == "0") {
       newValue = 0; // inverted
       stat[4] = stat0;
    } else {
       newValue = 1; // inverted
       stat[4] = stat1;
    }
    digitalWrite(pin[4],newValue);
    pubStatusWS(sTopic[4], stat[4],true);
 } else if (Topic_is == sTopic[5] + "/control") {
    String x = payload_is;
    analogWrite(pin[5],x.toInt());
    stat[5] = setStatus(x);
    pubStatusWS(sTopic[5], stat[5],true);
 } else if (Topic_is == sTopic[6] + "/control") {
    String x = payload_is;
    analogWrite(pin[6],x.toInt());
    stat[6] = setStatus(x);
    pubStatusWS(sTopic[6], stat[6],true);
 } else if (Topic_is == sTopic[7] + "/control") {
    String x = payload_is;
    analogWrite(pin[7],x.toInt());
    stat[7] = setStatus(x);
    pubStatusWS(sTopic[7], stat[7],true);
 } else if (Topic_is == prefix + "/ids") {
    ids = payload_is;
 } else if (Topic_is == prefix) {
    if (payload_is == "HELLO") {
      pubConfigWS();
    }
 }

   
}

void pubStatusWS(String topic_pub, String stat_pub,bool save_eeprom) {//отправка на сервер 

    DynamicJsonBuffer jsonBufferParse;
    JsonObject& jsonParse = jsonBufferParse.parseObject(stat_pub);//сперва раскодируем
    
    // Test if parsing succeeds.
    if (!jsonParse.success()) {
    Serial.println("parseObject() failed");
    return;
    }
    const char* stat = jsonParse["status"];

    DynamicJsonBuffer jsonBufferCreate;
    JsonObject& createJson = jsonBufferCreate.createObject();
    createJson["sTopic"]=topic_pub;
    createJson["status"] = stat;
    char buffer[250] = "";
    createJson.printTo(buffer, sizeof(buffer));

    
    if ( webSocket.sendTXT(NumberWebSocket,buffer)) { 
      if (save_eeprom){
           //int stat_int = atoi( stat );
           //saveEEPROM(topic_pub,stat_int);
           if (saveSPIFFS()){
             //saveSPIFFSSerial.println("saved:"+topic_pub+"status"+stat_int);
            };
      }
       Serial.println("Publish new statusWS for " + topic_pub + ", value: " + stat);
    } else {
       Serial.println("Publish new statusWS for " + topic_pub + " FAIL!");
    }
}

void pubConfigWS(){

  bool success;
  String buffer = prefix + "/" + deviceID + "/config";
  success = webSocket.sendTXT(NumberWebSocket,buffer);
  if (success) {
      delay(500);
      for (int i = 0; i < nWidgets; i = i + 1) {
        webSocket.sendTXT(NumberWebSocket,thing_config[i]);
        if (success) {
          Serial.println("Publish config WS: Success (" + thing_config[i] + ")");
        } else {
          Serial.println("Publish config WS FAIL! ("    + thing_config[i] + ")");
        }
        delay(50);
      }      
  }
    if (success) {
     Serial.println("Publish configWebscoket: Success");
  } else {
     Serial.println("Publish config: FAIL");
  }
  for (int i = 0; i < nWidgets; i = i + 1) {
    pubStatusWS(sTopic[i], stat[i],false);
    delay(50);
  } 
//  createJson.printTo(Serial);

  }
