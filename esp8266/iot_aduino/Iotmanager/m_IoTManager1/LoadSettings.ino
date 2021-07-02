bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/settings.txt", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open config file");
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

  // загружаем файл конфигурации в глобальную переменную
  String  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root
  /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  /////////////////////////////

  const char *buff_softAP_ssid = root["softAP_ssid"]; strncpy(softAP_ssid, buff_softAP_ssid, sizeof(buff_softAP_ssid));
  const char *buff_softAP_password = root["softAP_password"]; strncpy(softAP_password, buff_softAP_password, sizeof(buff_softAP_password));
  const char *buff_ssid = root["ssid"]; strncpy(ssid, buff_ssid, sizeof(buff_ssid)); delete buff_ssid;
  const char *buff_password = root["password"]; strncpy(password, buff_password, sizeof(buff_password));

  //const char *buff_password = root["password"];strncpy(password, buff_password, sizeof(buff_password));
  /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  //char *myHostname = "esp8266";

  prefix = root["prefix"].as<String>();
  deviceID = root["deviceID"].as<String>();

  mqttServerName = root["mqttServerName"].as<String>();
  mqttport = root["timezone"];
  mqttuser = root["mqttuser"].as<String>();
  mqttpass = root["mqttpass"].as<String>();

  const char *buff_smtp_arr = root["smtp_arr"]; strncpy(smtp_arr, buff_smtp_arr, sizeof(buff_smtp_arr));
  smtp_port = root["smtp_port"];
  to_email_addr = root["to_email_addr"].as<String>();
  from_email_addr = root["from_email_addr"].as<String>();
  emaillogin = root["emaillogin"].as<String>();
  password_email = root["password_email"].as<String>();


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
    Serial.println("Failed to open config file for writing");
    return false;
  }
  json.printTo(configFile);
  json.printTo(Serial);

  //  json.printTo(jsonConfig);
  // Открываем файл для записи
  configFile.close();
  return true;
}
bool savePinSetup(String json) {
  File configFile = SPIFFS.open("/pin_setup_real.txt", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  configFile.print(json);
  return true;
}
void updatepinsetup(String jsonrecieve) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(jsonrecieve);
  unsigned int numberChosed = rootjs["numberChosed"];
  if (numberChosed > 32) {
    numberChosed = 32;
  }
  for (int i = 0; i < numberChosed; i++) {
    String pinmodeJS = rootjs["pinmode"][i];
    String idJS = rootjs["id"][i];
    int pinJS = rootjs["pin"][i];
    String pageJS = rootjs["page"][i];
    String descrJS = rootjs["descr"][i];
    String widgetJS = rootjs["widget"][i];
    String defaultValJS = rootjs["defaultVal"][i];
    if (pinmodeJS) {
      pinmode[i] = pinmodeJS;
    }
    if (idJS) {

    }
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
      defaultVal[i] = defaultValJS;
    }
  }
  Serial.println();
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(id[i]);
  }
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(pin[i]);
  }
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(page[i]);
  }
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(descr[i]);
  }
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(widget[i]);
  }
  for (int i = 0; i < numberChosed; i++) {
    Serial.print(defaultVal[i]);
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
