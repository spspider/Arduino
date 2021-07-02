bool loadConfig(String  jsonConfig) {
  // Открываем файл для чтения
  /*
    File configFile = SPIFFS.open("/settings.txt", "r");
    if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open settings.txt");
    //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    configFile.close();
    }
    // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
    size_t size = configFile.size();
    if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    }
  */



  // Serial.println(jsonConfig);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  if (!root.success()) {
    Serial.println("Parsing fail");
    return false;
  }
  /////////////////////////////

  if (root.containsKey("softAP_ssid")) {
    /*
      String softAP_password_string = root["softAP_password"].as<String>();
      softAP_password_string.toCharArray(softAP_password, 32);
    */
    //const char *buff_softAP_ssid = root["softAP_ssid"]; softAP_ssid = (char[32])buff_softAP_ssid;
    //const char *buff_softAP_password = root["softAP_password"]; softAP_password = (char[32])buff_softAP_password;
    //Serial.print("softAP_ssid"); Serial.println(softAP_ssid);
    //Serial.print("softAP_password"); Serial.println(softAP_password);
  }
  if (root.containsKey("iot_enable")) {
    IOT_Manager_loop = root["iot_enable"];
    if (IOT_Manager_loop) {
      client.disconnect();
    }
  }
  if (root.containsKey("ssid")) {
    String ssid_string = root["ssid"].as<String>();
    ssid_string.toCharArray(ssid, 32);
    String password_string = root["password"].as<String>();
    password_string.toCharArray(password, 32);
    /*
      String softAP_password_string = root["softAP_password"].as<String>();
      softAP_password_string.toCharArray(password, 32);
    */
    //const char *buff_ssid = root["ssid"]; buff_ssid = (char*)ssid;
    //const char *buff_password = root["password"]; buff_password = (char*)password;
  }

  /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  /*
     if (root["esp8266"]) {
    String myHostname_str = root["esp8266"].as<String>();
    myHostname_str.toCharArray(myHostname, myHostname_str.length() + 1);
    }
  */
  if (root.containsKey("prefix")) {
    prefix = root["prefix"].as<String>();
  }
  if (root.containsKey("deviceID")) {
    String softAP_ssid_string = root["deviceID"].as<String>();
    softAP_ssid_string.toCharArray(softAP_ssid, 32);
    deviceID = root["deviceID"].as<String>();
    //const char *buff_softAP_ssid = root["deviceID"]; //snprintf(softAP_ssid, sizeof softAP_ssid, "%s", buff_softAP_ssid); // strncpy(smtp_arr, buff_smtp_arr, strlen(buff_smtp_arr));
    //softAP_ssid=copy(buff_softAP_ssid);
    //softAP_ssid[sizeof(softAP_ssid) - 1] = '\0';
    //softAP_ssid = string2char(deviceID);
  }

  mqttServerName = root["mqttServerName"].as<String>();
  root.containsKey("mqttport") ? mqttport = root["mqttport"] : mqttport = 1883;
  mqttuser = root["mqttuser"].as<String>();
  mqttpass = root["mqttpass"].as<String>();

  const char *buff_smtp_arr = root["smtp_arr"]; snprintf(smtp_arr, sizeof smtp_arr, "%s", buff_smtp_arr); // strncpy(smtp_arr, buff_smtp_arr, strlen(buff_smtp_arr));
  smtp_arr[sizeof(smtp_arr) - 1] = '\0';

  //String smtp_arr_str = root["smtp_arr"]; strcpy(smtp_arr, smtp_arr_str.c_str());

  smtp_port = root["smtp_port"];
  to_email_addr = root["to_email_addr"].as<String>();
  from_email_addr = root["from_email_addr"].as<String>();
  emaillogin = (root["emaillogin"].as<String>());
  password_email =  (root["password_email"].as<String>());

  root.containsKey("ipport") ? ipport = root["ipport"] : ipport = 80;
  root.containsKey("mqttspacing") ? mqttspacing = root["mqttspacing"] : mqttspacing = 60;
  root.containsKey("timezone") ? timezone = root["timezone"] : timezone = 2;
  root.containsKey("geo_enable") ? geo_enable = root["geo_enable"] : geo_enable = 0;
  setup_IOTManager();

  return true;
}
/*
  char* copy(const char* orig) {
  char *res = new char[strlen(orig) + 1];
  strcpy(res, orig);
  return res;
  }
*/
char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}
String readCommonFiletoJson(String file) {
  File configFile = SPIFFS.open("/" + file + ".txt", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open " + file + ".txt");
    configFile.close();
    return "";
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    //configFile.close();
    //return "";
  }
  String  jsonConfig = configFile.readString();
  //server.send(200, "text/json", jsonConfig);
  Serial.println("READ:");
  Serial.println(jsonConfig);
  configFile.close();
  return jsonConfig;
}
bool saveCommonFiletoJson(String pagename, String json) {

  File configFile = SPIFFS.open("/" + pagename + ".txt", "w");
  if (!configFile) {
    Serial.println("Failed to open " + pagename + ".txt for writing");
    return false;
  }
  Serial.println("SAVE:");
  Serial.println(json);
  configFile.print(json);
  configFile.close();
  return true;
}
bool loadWidgets() {
  String  jsonConfig = readCommonFiletoJson("pin_setup");
  /*
    File configFile = SPIFFS.open("/pin_setup.txt", "r");
    if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open pin_setup.txt");
    configFile.close();
    return false;
    }
    // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
    size_t size = configFile.size();
    if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
    }
    String  jsonConfig = configFile.readString();
  */
  updatepinsetup(jsonConfig);
  return true;
}

/////////////////////CONDITION////////////////////////////////////////////////////
bool SaveCondition(String json) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(json);

  unsigned int NumberID = rootjs["ID"];

  Serial.println("NimberId:" + NumberID);
  String NameFile = "Condition" + String(NumberID);
  saveCommonFiletoJson(NameFile, json);
  /*
    File configFile = SPIFFS.open("/Condition" + String(NumberID) + ".txt", "w");
    if (!configFile) {
    Serial.println("Failed to open /Condition" + String(NumberID) + ".txt for writing");
    return false;
    }
    configFile.print(json);
  */
  //load_Current_condition(NumberID);//сразу же загружаем в перменные это условие
  return true;
}
/*
  String LoadCondition(unsigned int NumberID) {

  String jsonConfig = readCommonFiletoJson(NameFile);

    File configFile = SPIFFS.open("/Condition" + String(NumberID) + ".txt", "r");
    if (!configFile) {
     // если файл не найден
     Serial.println("Failed to open /Condition" + String(NumberID) + ".txt");
     configFile.close();
     return "null";
    }
    // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
    size_t size = configFile.size();
    if (size > 1024) {
     Serial.println("Config file size is too large");
     configFile.close();
     return "null";
    }
    String  jsonConfig = configFile.readString();

  //updatepinsetup(jsonConfig);
  return jsonConfig;
  }
*/
//////////////////////////////////////////////////////////////////////////////
bool updatepinsetup(String jsonrecieve) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(jsonrecieve);
  unsigned char numberChosed = rootjs["numberChosed"];
  if (numberChosed == 0) {
    Serial.println("FAIL!! numberChosed = 0");
    return false;
  }
  if (numberChosed > nWidgetsArray) {
    numberChosed = nWidgetsArray;
  }
  nWidgets = numberChosed;

  for (unsigned char i = 0; i < numberChosed; i++) {
    char pinmodeJS = rootjs["pinmode"][i];
    char idJS = rootjs["id"][i];
    char pinJS = rootjs["pin"][i];
    String pageJS = rootjs["page"][i];
    String descrJS = rootjs["descr"][i];
    char widgetJS = rootjs["widget"][i];
    unsigned int defaultValJS = rootjs["defaultVal"][i];
    //int delimValJS = rootjs["aDiv"];
    char IrBtnId  = rootjs["IrBtnId"][i];
    if (pinmodeJS) {
      pinmode[i] = pinmodeJS;
      //Serial.println(pinmodeJS);
    }
    id[i] = i;
    pin[i] = pinJS;
    if (descrJS) {
      descr[i] = descrJS;
    }
    if (widgetJS) {
      widget[i] = widgetJS;
    }
    if (IrBtnId) {
      IrButtonID[i] = IrBtnId;
      //Serial.println("IrButtonID[i]"+String(IrBtnId));
    }
    /*
      if (defaultValJS) {
      if (pinmodeJS != 6) {//IR
        defaultVal[i] = defaultValJS;
      }
      }
    */
    //Serial.println("id:" + String( id[i],DEC) + "pin:" + String(  pin[i]) + "descr:" + String( descr[i]) + "widget:" + String( widget[i]) + "IrButtonID:" + String( IrButtonID[i]) + "defaultVal:" + String( defaultVal[i]));
  }
  //RECV_PIN = rootjs["IR_rec"];
  //SEND_PIN = rootjs["IR_LED"];
  analogDivider = rootjs["aDiv"];
  analogSubtracter = rootjs["aSusbt"];
  pwm_delay_long = rootjs["PWM_interval"];

  initThingConfig();
  /////////////////////////
  DynamicJsonBuffer jsonBuffer_stat;
  String stat1 = readCommonFiletoJson("stat");
  JsonObject& root_stat = jsonBuffer_stat.parseObject(stat1);

  for (char i = 0; i < numberChosed; i++) {
    int stat_js = root_stat["stat"][i];
    if (stat_js) {
      stat[i] = stat_js;
    }
    //Serial.println(stat_js);
  }
  ////////////////////////////
  Setup_pinmode();
  return true;
}

