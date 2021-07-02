// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!




/*
  void WebScoketCallback(String text) {
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
  callback_scoket(Topic_is, newValue);
  }
*/
void callback_scoket(char i, int payload_is) {
  bool that_Ajax = false;
  bool saveEEPROM = false;

  stat[i] = (payload_is);
  if ((pinmode[i] == 2) || (pinmode[i] == 1)) { //out,in - saveEEPROM=false;
    if (widget[i] == 1) {//toggle
    } else if (widget[i] == 2) {//simple-btn

    }
    //Serial.println("SendIR");
    send_IR(i);
    digitalWrite(pin[i], payload_is);
    check_if_there_timer_once(i);
  } else if (pinmode[i] == 3) {//pwm
    analogWrite(pin[i], payload_is);
  }
  else if (pinmode[i] == 5) {//low_pwm
    //String x = payload_is;
    low_pwm[i] = payload_is;

  }
  else if (pinmode[i] == 4) { //adc
  }
  //}

  //}

  check_for_changes();
  pubStatusFULLAJAX_String(false);

}


void pubStatusFULLAJAX_String(bool save_eeprom) { //отправка на сервер _nobuffer
  String stat1 = "{\"stat\":[";
  for (char i = 0; i < nWidgets; i++) {
    float that_stat = 0.0F;
    that_stat = stat[i];
    if ((pinmode[i] == 1)) {//in
      that_stat = digitalRead(pin[i]);
      stat[i] = that_stat;
    }
    else if ((pinmode[i] == 3) || (pinmode[i] == 5) ) { //pwm не знаю как сделать,статус не обновляется, как прочитать analogWrite

    }
    else if (pinmode[i] == 4) {//adc
      that_stat = (analogRead(pin[i]) * 1.0F / analogDivider * 1.0F ) + analogSubtracter; //adc pin:A0
      stat[i] = (int)that_stat;
    }
    else if (pinmode[i] == 6) {//dht Temp
      that_stat = (dht.getTemperature());
    }
    else if (pinmode[i] == 7) {
      that_stat = (float)low_pwm_off;
    }
    else if (pinmode[i] == 8) {//dht Hum
      that_stat = (dht.getHumidity());
    }
    else {
    }
    if (isnan(that_stat)) {
      that_stat =  stat[i] = 0;
    }
    stat[i] = (int)that_stat;
    stat1 += String(that_stat, 2);
    stat1 += (i < nWidgets - 1) ? "," : "]";
  }
  stat1 += "}";
  String buffer = stat1;
  server.send(200, "text / json", buffer);
}

/*
  void pubStatusFULLAJAX_buffer(bool save_eeprom) { //отправка на сервер
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonArray& stat_array = json.createNestedArray("stat");
  for (int i = 0; i < nWidgets; i = i + 1) {
    //JsonObject& stat_array = networks.createNestedObject();
    int that_stat = -123;
    that_stat = stat[i];
    if ((pinmode[i] == 1) || (pinmode[i] == 2)) {//int,out
      that_stat = digitalRead(pin[i]);
      stat[i] = that_stat;
    }
    else if ((pinmode[i] == 3) || (pinmode[i] == 5) ) { //pwm не знаю как сделать,статус не обновляется, как прочитать analogWrite

    }
    else if (pinmode[i] == 4) {//adc
      int analog = (analogRead(pin[i])) ; //adc pin:A0
      that_stat = (analog / analogDivider) + analogSubtracter;
      stat[i] = that_stat;
    }
    else {

    }

    stat_array.add(String(that_stat));
  }
  Serial.print("nWidjets:" + String(nWidgets));
  json.printTo(Serial);
  String buffer;
  json.printTo(buffer);
  server.send(200, "text/json", buffer);
  }
*/
