


float get_new_pin_value(uint8_t i) {


  float that_stat = 0.0f;
  if (pin[i] == 255) {
    return that_stat;
  }
  //  if ((get_new_pin_value_) && (!license)) get_new_pin_value_ = false;
  that_stat = stat[i];
  switch (pinmode[i]) {
    case 1://in
      that_stat = digitalRead(pin[i])^defaultVal[i];
      stat[i] = that_stat;
      break;
    case 2://out
      // that_stat = digitalRead(pin[i])^defaultVal[i];
      //stat[i] = that_stat;
      break;
    case 3:
      break;
    case 4://adc
      if (!license)return 127;
      //that_stat = (analogRead(pin[i]) / analogDivider) + analogSubtracter; //adc pin:A0
      //that_stat = (analogRead(17) - analogSubtracter) / analogDivider * 1.0F;
      stat[i] = (int)that_stat;
      break;
    case 6://dht Temp
      if (!license)return 127;
      that_stat = (dht.getTemperature());
      that_stat == 0 ? that_stat = stat[i] : that_stat;
      break;
    case 7:
      if (!license)return 127;
      that_stat = (float)low_pwm_off;
      break;
    case 8://dht Hum
      if (!license)return 127;
      that_stat = (dht.getHumidity());
      that_stat == 0 ? that_stat = stat[i] : that_stat;
      break;
    case 9://remote
      if (!license)return 127;
      that_stat = getHttp(String(descr[i])).toFloat();
      break;
    case 11:
      if (!license)return 127;
      //compass
      //      that_stat = dimmer.getPower();

      //that_stat = (encoder.getAngle() - analogSubtracter) / analogDivider * 1.0F;


      break;
    case 12://MAC ADRESS
      //that_stat = stat[i] ^ 1;
      break;
    case 13://EncA
      that_stat = no_internet_timer;
      break;
    case 14://EncB
      //that_stat = stat[i] ^ 1;
      break;
    case 15://ads
      that_stat = (ads.readADC_SingleEnded(defaultVal[i]));
      break;
    case 10://PowerMeter должен быть последним, иначе ошибка jump to case label
      if (!license)return 127;
      // double Irms ;
      that_stat = (float) emon1.calcIrms(1480); // Calculate Irms only
      //that_stat = ((float)Irms * 1.0F);//  + analogSubtracter;
      that_stat = (that_stat * 1.0F / analogDivider) + analogSubtracter;
      //that_stat = (that_stat - analogSubtracter) / analogDivider * 1.0F;
      break;

      /*
        case 11:



        break;
      */
  }
  //that_stat = (isnan(that_stat) || isnanf (that_stat)) ? 0 : that_stat;
  if ((isnan(that_stat)) || ( isinf (that_stat))) {
    that_stat =  stat[i];//0
  }

  return that_stat;
}


void makeAres_sim(String json) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  char that_pin;
  float that_val = 0.0F;
  char control = 255;
  char that_stat = 255;
  //char that_nID;
  String String_value = "";
  root.containsKey("pin") ? that_pin = root["pin"] : that_pin = 255;
  root.containsKey("stat") ? that_stat = root["stat"] : that_stat = 255;
  //root.containsKey("read") ? that_stat = root["stat"] : that_stat = 255;
  root.containsKey("val") ? that_val = root["val"] : that_val = -1;
  //root.containsKey("nID") ? that_nID = root["nID"] : that_nID = 255;
  root.containsKey("C") ? control = root["C"] : control = 255;
  root.containsKey("st") ? String_value = root["st"].as<String>() : String_value = "";
  switch (control) {
    case 255: {
        char i = 255;
        for (char i1 = 0; i1 < nWidgets; i1++) {
          if (that_pin == pin[i1])
            i = i1;
          break;
        }

        if (that_stat != 255) {
          if  (root.containsKey("val")) {
            stat[that_stat] = that_val;
          } else {
            that_val = get_new_pin_value(that_stat);//только чтение
          }
        }
        if (i != 255) {
          if ((pinmode[i] == 2) || (pinmode[i] == 1)) {//out, in
            stat[i] = (int)that_val ^ defaultVal[i];
            //send_IR(i);
            digitalWrite(that_pin, stat[i]);
          }
          else if (pinmode[i] == 3) {//pwm
            //unsigned int freq = PWM_frequency * 100;
            //analogWriteFreq(freq);
            analogWrite(that_pin, that_val);
          }
        }

        //pubStatusFULLAJAX_String(false);
        that_val = round(that_val * 200) / 200;
        server.send(200, "text / json", String(that_val, DEC));
        break;
      }
    case 1://PLUS Control

      { 
        bySignalPWM[that_pin][that_stat] = that_val;


        //PWM_json.add(bySignalPWM[that_pin][that_nID]);
        //that_pin-это условие
        /*
          for (char i1 = 0; i1 < Condition; i1++) {
          //JsonArray& C = PWM_json.createNestedArray();
          for (char i = 0; i < NumberIDs[i1]; i++) {
            if (bySignalPWM[i1][i] != -1) {
              PWM_json.add(bySignalPWM[i1][i]);
            }
          }
          //PWM_json.add(Condition_json);
          }
        */
        //String buffer;
        //json.printTo(buffer);
        //Serial.println(buffer);
        server.send(200, "text / json",  saveConditiontoJson(that_pin));
        break;
      }
    case 2: { //IR
        send_IR(that_stat);
        break;
      }
    case 3: {
        //irsend.sendNEC(StrToHex(String_value.c_str()), 32);
        break;
      }
    case 4: {
        //        irsend.sendRaw(String_value, String_value.length(), 38);
        break;
      }
  }
}

