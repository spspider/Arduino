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
void saveEEPROM(String topic_pub,int new_stat){
  int that_widget =-1;
  for (int i =0;i<nWidgets;i++){
     //Serial.println("topic_pub:"+topic_pub+" sTopic:"+sTopic[i]);
    if (String(topic_pub) == String(sTopic[i])){

        that_widget = i;
        //if (defaultVal[i]==new_stat){that_widget=-1;}
        break;
        }
    }
    if (that_widget!=-1){
    //if (defaultVal[that_widget]!=new_stat){
      int adress=that_widget*5+addr_widgets_begins;
        Serial.println("saveEEPROM"+sTopic[that_widget]+" status:"+new_stat+" addr:"+adress);
      //String value
        EEPROM.begin(512);
        EEPROM.put(adress, new_stat);
        EEPROM.commit();
        EEPROM.end();
          //delay(100);
   /// }
        
      }
  }

    /////////////////////////////////////////////////////////////////////////////////////////
unsigned int *getSPIFFS_JSON_VALUE(int nWidgets){ 
      unsigned int value_back[nWidgets];
    for (int i=0;i<nWidgets;i++){
      value_back[i]=defaultVal[i];
      }

  File configFile = SPIFFS.open("/config.txt", "r");
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
    if (!json.success()) {
    Serial.println("Failed to parse config file");
    return value_back;
    }
    
    //unsigned int value_back[nWidgets];
    for (int i=0;i<nWidgets;i++){
    
        const char* value_char = json[sTopic[i]];
        StaticJsonBuffer<200> jsonBufferParseStatus;
        JsonObject& jsonParseSt = jsonBufferParseStatus.parseObject(value_char);//сперва раскодируем
        
        const char* status_json = jsonParseSt["status"];
        String status_str = String((char*)status_json);
        unsigned int status_int = status_str.toInt();
        if (jsonParseSt.success()) {
          value_back[i]=status_int;
        }else{
          value_back[i]=defaultVal[i];
        }
        
        Serial.print("stored_value:");
        
        Serial.println(value_back[i]);
        //Serial.println(status_int);
      }
        
  return   value_back;

  }
  /////////////////////////////////////////////////////////////////////////////////////
    bool saveSPIFFS() {
      //int id=-1;

    
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  for (int i=0;i<nWidgets;i++){
    
     
   String id_t = sTopic[i];
   json[id_t] = stat[i];
  
  //const char* id_t="id:"+i;
  //json[id_t] = value;
  }
  
  File configFile = SPIFFS.open("/config.txt", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
 //Serial.println("Saved:"+id_t+"value"+defaultVal[i]);
  json.printTo(configFile);
  json.printTo(Serial);
  return true;
}
////////////////////////////////////////////////////////////////////////////////
 unsigned int getEEPROM(int id){
    int value_back=0;
     int adress=id*5+addr_widgets_begins;
  EEPROM.begin(512);
  delay(100);
  EEPROM.get(adress,value_back);
    delay(500);
  Serial.print("adress:");
  Serial.print(adress);
    Serial.print(" id:");
  Serial.print(id);
  Serial.println(" RestoredValue:"+value_back);
  delay(500);
    EEPROM.end();

  return value_back;
  }
