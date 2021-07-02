bool loadConfig(String  jsonConfig) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  if (!root.success()) {
    //Serial.println("Parsing fail");
    return false;
  }
  /////////////////////////////
  if (root.containsKey("ssid")) {
    String ssid_string = root["ssid"].as<String>();
    ssid_string.toCharArray(ssid, 32);
    String password_string = root["password"].as<String>();
    password_string.toCharArray(password, 32);
  }
  if (root.containsKey("softAP")) {
    String softAP_ssid_string = root["softAP"].as<String>();
    String softAP_password_String = root["softAP_pass"].as<String>();
    softAP_ssid_string.toCharArray(softAP_ssid, 32);
    softAP_password_String.toCharArray(softAP_password, 32);
    Serial.print("softAP_ssid:");
    Serial.println(softAP_ssid);
    Serial.print("softAP_password:");
    Serial.println(softAP_password);
    //deviceID = root["deviceID"].as<String>();
  }
  /*
     char lock_pin = 16;
    char lock_pin_invert = 0;
    char open_timer = 3;
    int count_timer_lock_anyway = 10;
  */
  if (root.containsKey("lock_pin")) {
    lock_pin = root["lock_pin"];
    Serial.print("lock_pin:");
    Serial.println(lock_pin, DEC);
  }
  if (root.containsKey("lock_pin_invert")) {
    lock_pin_invert = root["lock_pin_invert"];
    Serial.print("lock_pin_invert:");
    Serial.println(lock_pin_invert, DEC);
  }
  if (root.containsKey("open_timer")) {
    open_timer = root["open_timer"];
    Serial.print("open_timer:");
    Serial.println(open_timer, DEC);
  }
  if (root.containsKey("count_timer_lock_anyway")) {
    count_timer_lock_anyway = root["count_timer_lock_anyway"];
    Serial.print("count_timer_lock_anyway:");
    Serial.println(count_timer_lock_anyway, DEC);
  }
  if (root.containsKey("w433rcv")) {
    w433rcv = root["w433rcv"];
    Serial.print("w433rcv:");
    Serial.println(w433rcv, DEC);
  }
  root.containsKey("reg") ? register_is_on = root["reg"] : register_is_on = 0;
  root.containsKey("login_Authentication") ? strcpy(login_Authentication, root["login_Authentication"]) : 0;
  root.containsKey("pass_Authentication") ? strcpy(pass_Authentication, root["pass_Authentication"]) : 0;
  root.containsKey("open_type") ? open_type = root["open_type"] : 0;
  root.containsKey("connectAsAp") ? connectAsAp = root["connectAsAp"] : 1;
  root.containsKey("wgD0") ? wgD0 = root["wgD0"] : 1;
  root.containsKey("wgD1") ? wgD1 = root["wgD1"] : 1;

  serverWG = (root["serverWG"].as<String>());
  return true;
}
void updateSettings() {
  String settings = readCommonFiletoJson("setup_lock");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(settings);
  root["reg"] = register_is_on;
  String buffer;
  root.printTo(buffer);
  saveCommonFiletoJson("setup_lock", buffer, 1);
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
  }
  String  jsonConfig = configFile.readString();
  Serial.println(jsonConfig);
  configFile.close();
  return jsonConfig;
}
bool saveCommonFiletoJson(String pagename, String json, boolean write_add) {
  //w-перезапись, а  - добавление
  //  char *write_add_char = (write_add == true) ? 'w' : 'a';
  File configFile;
  if (write_add == 1) {
    configFile = SPIFFS.open("/" + pagename + ".txt", "w");
  } else if (write_add == 0) {
    configFile = SPIFFS.open("/" + pagename + ".txt", "a");
    if (!configFile) {
      configFile = SPIFFS.open("/" + pagename + ".txt", "w");
    }
  }
  if (!configFile) {
    Serial.println("Failed to open " + pagename + ".txt for writing");
    return false;
  }
  Serial.println("SAVE: " + String(write_add, DEC));
  Serial.println(json);
  configFile.print(json);
  configFile.close();
  return true;
}
