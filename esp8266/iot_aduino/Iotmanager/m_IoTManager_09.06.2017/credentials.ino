/** Load WLAN credentials from EEPROM */
const int addr_widgets_begins=64;
void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0+sizeof(ssid), password);
  char ok[2+1];
  EEPROM.get(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password)>0?"********":"<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0+sizeof(ssid), password);
  char ok[2+1] = "OK";
  EEPROM.put(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}
void saveEEPROM(String topic_pub,String new_stat){
  int that_widget =-1;
  for (int i =0;i<nWidgets;i++){
     //Serial.println("topic_pub:"+topic_pub+" sTopic:"+sTopic[i]);
    if (String(topic_pub) == String(sTopic[i])){
        
        that_widget = i;
        if (defaultVal[i]==new_stat){that_widget=-1;}
        break;
        }
    }
    if (that_widget!=-1){
    if (defaultVal[that_widget]!=new_stat){
      int adress=that_widget*5+addr_widgets_begins;
        Serial.println("saveEEPROM"+sTopic[that_widget]+" status:"+new_stat+" addr:"+adress);
      //String value
        //EEPROM.begin(512);
        //EEPROM.put(adress, new_stat);
        //EEPROM.commit();
        //EEPROM.end();
    }
        
      }
  }
 unsigned int getEEPROM(int id){
    int value_back=0;
     int adress=id*5+addr_widgets_begins;
  EEPROM.begin(512);
  EEPROM.get(adress, value_back);
  Serial.print("id:");
  Serial.print(id);
  Serial.println("RestoredValue:"+value_back);
  return value_back;
  }
