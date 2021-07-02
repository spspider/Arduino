bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/settings.txt", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open settings.txt");
    //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
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
  configFile.close();
  Serial.println(jsonConfig);
  /*
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root
  /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  /////////////////////////////
/*
  const char *buff_softAP_ssid = root["softAP_ssid"]; strncpy(softAP_ssid, buff_softAP_ssid, sizeof(softAP_ssid));
  const char *buff_softAP_password = root["softAP_password"]; strncpy(softAP_password, buff_softAP_password, sizeof(softAP_password));
  const char *buff_ssid = root["ssid"]; strncpy(ssid, buff_ssid, sizeof(ssid));// delete buff_ssid;
  const char *buff_password = root["password"]; strncpy(password, buff_password, sizeof(password));

  Serial.print("softAP_ssid:");
  Serial.println(softAP_ssid);

  //const char *buff_password = root["password"];strncpy(password, buff_password, sizeof(buff_password));
  /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  //char *myHostname = "esp8266";
/*
  prefix = root["prefix"].as<String>();
  deviceID = root["deviceID"].as<String>();

  mqttServerName = root["mqttServerName"].as<String>();
  mqttport = root["mqttport"];
  mqttuser = root["mqttuser"].as<String>();
  mqttpass = root["mqttpass"].as<String>();

  const char *buff_smtp_arr = root["smtp_arr"]; strncpy(smtp_arr, buff_smtp_arr, sizeof(buff_smtp_arr));
  smtp_port = root["smtp_port"];
  to_email_addr = root["to_email_addr"].as<String>();
  from_email_addr = root["from_email_addr"].as<String>();
  emaillogin = root["emaillogin"].as<String>();
  password_email = root["password_email"].as<String>();
  Serial.println("settings.txt data restored: " + jsonConfig);
  */
  return true;
}

String readCommonFiletoJson(String file) {
  File configFile = SPIFFS.open("/" + file, "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open " + file);
    configFile.close();
    return "";
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return "";
  }
  String  jsonConfig = configFile.readString();
  //server.send(200, "text/json", jsonConfig);
  return jsonConfig;
}
bool loadWidgets() {
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
  updatepinsetup(jsonConfig);
  return true;
}
bool saveConfig() {
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  JsonObject& json = jsonBuffer.createObject();
  // Заполняем поля json

  json["softAP_ssid"] = softAP_ssid;
  json["softAP_password"] = softAP_password;
  json["ssid"] = ssid;
  json["password"] = password;

  ///////////////////////
  json["mqttServerName"] = mqttServerName;
  json["mqttport"] = mqttport;
  json["mqttuser"] = mqttuser;
  json["mqttpass"] = mqttpass;
  json["prefix"] = prefix;
  json["deviceID"] = deviceID;
  ////////////////////////////

  json["smtp_arr"] = smtp_arr;
  json["smtp_port"] = smtp_port;
  json["to_email_addr"] = to_email_addr;
  json["from_email_addr"] = from_email_addr;
  json["emaillogin"] = emaillogin;
  json["password_email"] = password_email;

  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  File configFile = SPIFFS.open("/settings.txt", "w");
  if (!configFile) {
    Serial.println("Failed to open settings.txt for write");
    return false;
  }
  json.printTo(configFile);
  json.printTo(Serial);

  //  json.printTo(jsonConfig);
  // Открываем файл для записи
  configFile.close();
  return true;
}
bool saveIRsetup(String json) {

  File configFile = SPIFFS.open("/IRButtons.txt", "w");
  if (!configFile) {
    Serial.println("Failed to open IRButtons.txt for writing");
    return false;
  }
  configFile.print(json);
  return true;
}
bool saveSignal(String json, String pagename) {

  File configFile = SPIFFS.open("/" + pagename + ".txt", "w");
  if (!configFile) {
    Serial.println("Failed to open " + pagename + ".txt for writing");
    return false;
  }
  configFile.print(json);
  return true;
}
bool savePinSetup(String json) {
  DynamicJsonBuffer jsonBuffer;//для теста
  JsonObject& rootjs = jsonBuffer.parseObject(json);//для теста
  File configFile = SPIFFS.open("/pin_setup.txt", "w");
  if (!configFile) {
    Serial.println("Failed to open pin_setup.txt for writing");
    return false;
  }
  configFile.print(json);
  return true;
}
/////////////////////CONDITION////////////////////////////////////////////////////
bool SaveCondition(String json) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(json);

  unsigned int NumberID = rootjs["ID"];

  Serial.println("NimberId:" + NumberID);
  File configFile = SPIFFS.open("/Condition" + String(NumberID) + ".txt", "w");
  if (!configFile) {
    Serial.println("Failed to open /Condition" + String(NumberID) + ".txt for writing");
    return false;
  }
  configFile.print(json);
  return true;
}
String LoadCondition(unsigned int NumberID) {
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
//////////////////////////////////////////////////////////////////////////////
void updatepinsetup(String jsonrecieve) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(jsonrecieve);
  unsigned int numberChosed = rootjs["numberChosed"];
  if (numberChosed > 32) {
    numberChosed = 32;
  }
  nWidgets = numberChosed;
  for (int i = 0; i < numberChosed; i++) {
    String pinmodeJS = rootjs["pinmode"][i];
    int idJS = rootjs["id"][i];
    int pinJS = rootjs["pin"][i];
    String pageJS = rootjs["page"][i];
    String descrJS = rootjs["descr"][i];
    String widgetJS = rootjs["widget"][i];
    unsigned int defaultValJS = rootjs["defaultVal"][i];
    if (pinmodeJS) {
      pinmode[i] = pinmodeJS;
    }
    // if (idJS) {
    //  id[i] = idJS;
    //}
    // else {
    id[i] = i;
    //}
    if (pinJS) {
      pin[i] = pinJS;
    }
    if (pageJS) {
      page[i] = pageJS;
    }
    if (descrJS) {
      descr[i] = descrJS;
    }
    if (widgetJS) {
      widget[i] = widgetJS;
    }
    if (defaultValJS) {
      if (pinmodeJS != "IR") {
        defaultVal[i] = defaultValJS;
      }
    }
  }
  Serial.println();
  Serial.print("pinmode");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + pinmode[i]);
  }
  Serial.print("id");

  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + String(id[i]));
  }
  Serial.print("pin");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + String(pin[i]));
  }
  Serial.print("page");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + page[i]);
  }
  Serial.print("descr");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + descr[i]);
  }
  Serial.print("widget");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + widget[i]);
  }
  Serial.print("defaultVal");
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(" " + String(defaultVal[i]));
  }
  Serial.println();
  initThingConfig();
}

void handle_ConfigJSON() {
  //String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.createObject();
  // Заполняем поля json

  json["softAP_ssid"] = softAP_ssid;
  json["softAP_password"] = softAP_password;
  json["ssid"] = ssid;
  json["password"] = password;

  json["ip"] = WiFi.localIP().toString();

  json["mqttServerName"] = mqttServerName;
  json["mqttport"] = mqttport;
  json["mqttuser"] = mqttuser;
  json["mqttpass"] = mqttpass;
  json["prefix"] = prefix;
  json["deviceID"] = deviceID;

  json["smtp_arr"] = smtp_arr;
  json["smtp_port"] = smtp_port;
  json["to_email_addr"] = to_email_addr;
  json["from_email_addr"] = from_email_addr;
  json["emaillogin"] = emaillogin;
  json["password_email"] = password_email;

  json["time"] = GetTime();
  json["date"] = GetDate();
  // Помещаем созданный json в переменную root
  String root = "";
  json.printTo(root);
  server.send(200, "text/json", root);
}
void handle_ConfigJSON_pinSetup() {
  File configFile = SPIFFS.open("/pin_setup.txt", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open pin_setup.txt");
    configFile.close();
    //return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    //return false;
  }


  String  jsonConfig = configFile.readString();
  server.send(200, "text/json", jsonConfig);
  //updatepinsetup(jsonConfig);
}
/*
void handle_ConfigJSON_pinSetup2() {
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.createObject();
  JsonArray& pinmode_j = json.createNestedArray("pinmode");
  JsonArray& id_j = json.createNestedArray("id");
  JsonArray& pin_j = json.createNestedArray("pin");
  JsonArray& page_j = json.createNestedArray("page");
  JsonArray& descr_j = json.createNestedArray("descr");
  JsonArray& widget_j = json.createNestedArray("widget");
  JsonArray& defaultVal_j = json.createNestedArray("defaultVal");
  for (int i = 0; i < nWidgets; i++) {
    pinmode_j.add(pinmode[i]);
    id_j.add(id[i]);
    pin_j.add(pin[i]);
    page_j.add(page[i]);
    descr_j.add(descr[i]);
    widget_j.add(widget[i]);
    defaultVal_j.add(defaultVal[i]);

  }
  //id.add(double_with_n_digits(2.302038, 6));

  String root = "";
  json.printTo(root);
  Serial.print("serial send:");
  json.printTo(Serial);
  Serial.println();
  server.send(200, "text/json", root);
}
*/
