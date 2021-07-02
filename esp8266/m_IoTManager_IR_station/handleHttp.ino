/** Handle root or redirect to captive portal */
void handleRoot() {
  //  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
  //   return;
  // }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

  server.sendContent(
    "<html><head>"
    "<meta name='viewport' http-equiv='Content-Type' content='width=device-width, initial-scale=1; charset=utf-8'/>"
    "<link sync rel='stylesheet' href='/bootstrap.min.css?>"
    "<link sync rel='stylesheet' type='text/css' href='/style.css?'>"
    "</head><body>"
    "<div class='container'>"
    "<div class='row' style='text-align:center;'>"
    "<h1 style='margin:50px;'>Настройка роутера</h1>"
    "<div class='col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6'>"
  );
  /*
    if (server.client().localIP() == apIP) {
      server.sendContent(String("<p>Вы подключены без точки доступа, напрямую: ") + softAP_ssid + "</p>");
    } else {
      server.sendContent(String("<p>Вы подключены к сети: ") + ssid + "</p>");
    }
  */
  server.sendContent("<a class='btn btn-block btn-default' href='/wifi'>подключение к Wifi</a>");
  server.sendContent("<a class='btn btn-block btn-default' href='/other_setup'>настройки подключения</a>");
  server.sendContent("<a class='btn btn-block btn-default' href='/pin_setup'>настройки кнопок</a>");
  server.sendContent("<a class='btn btn-block btn-default' href='/IR_setup'>запись ИК</a>");
  server.sendContent("<a class='btn btn-block btn-default' href='/condition'>установка условий</a>");
  server.sendContent("<a class='btn btn-block btn-default' href='/home'>управление</a>");
  server.sendContent("</div></div></div></body></html>");
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
/*
  boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
  }
*/
/** Wifi config page handler */
void handleWifilist() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = ssid;
  json["WiFilocalIP"] = toStringIp(WiFi.localIP());
  json["softAP"] = softAP_ssid;
  json["WiFisoftAPIP"] = String(WiFi.softAPIP());


  JsonArray& scan_array = json.createNestedArray("scan");
  JsonArray& enc_array = json.createNestedArray("enc");
  JsonArray& RSSI_array = json.createNestedArray("RSSI");
  int n = WiFi.scanNetworks();
  if (n > 0) {
    json["n"] = n;
    for (int i = 0; i < n; i++) {
      scan_array.add(WiFi.SSID(i));
      enc_array.add(String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? +"Free" : ""));
      RSSI_array.add(WiFi.RSSI(i));
    }
  }
  String buffer;
  json.printTo(buffer);
  server.send(200, "text/json", buffer);

  /*
    String returnJson = "{";
    if (server.client().localIP() == apIP) {
    returnJson += "\"softAP\":";
    returnJson += "\"" + String(softAP_ssid) + "\"";
    } else {
    returnJson += "\"ssid\":";
    returnJson += "\"" + String(ssid) + "\"";
    }
    returnJson += ",\"softAP_ssid\":";
    returnJson += "\"" + String(softAP_ssid) + "\"";

    returnJson += ",\"WiFisoftAPIP\":";
    returnJson += "\"" + toStringIp(WiFi.softAPIP()) + "\"";

    returnJson += ",\"ssid\":";
    returnJson += "\"" + String(ssid);

    returnJson += ",\"WiFilocalIP\":";
    returnJson += "\"" + toStringIp(WiFi.localIP()) + "\"";

    //Serial.println("scan start");
    int n = WiFi.scanNetworks();
    returnJson += ",\"n\":";
    returnJson += "\"" + String(n) + "\"";
    returnJson += ",\"scan\":[";
    //Serial.println("scan done");
    if (n > 0) {
    for (int i = 0; i < n; i++) {
      returnJson += "\"" + WiFi.SSID(i) + "\"";
      returnJson += i < n - 1 ? "," : "]";
    }
    returnJson += ",\"enc\"=[";
    for (int i = 0; i < n; i++) {
      returnJson += "\"" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? +"Free" : "") + "\"";
      returnJson += i < n - 1 ? "," : "]";
    }
    returnJson += ",\"RSSI\":[";
    for (int i = 0; i < n; i++) {
      returnJson += WiFi.RSSI(i);//jkkggjujyuukkj
      returnJson += i < n - 1 ? "," : "]";
    }
    }
    returnJson += "}";

    Serial.println(returnJson);

    server.send(200, "text/json", returnJson);
  */
}
void handleWifi1() {
  server.sendHeader("Cache-Control", "public");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

  server.sendContent(
    "<html><head></head><body>"
    "<meta name='viewport' http-equiv='Content-Type' content='width=device-width, initial-scale=1; charset=utf-8'/>"
    "<link rel='stylesheet' href='/bootstrap.min.css?>"
    "<link rel='stylesheet' type='text/css' href='/style.css?'>"
    "<div class='container'>"
    "<div class='row' style='text-align:center;'>"
    "<h1 style='margin:50px;'>Настройка WIFI</h1>"
    "<div class='col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6'>"
  );

  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>Вы подключены без точки доступа, напрямую: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>Вы подключены к сети:  ") + ssid + "</p>");
  }
  server.sendContent(
    "\r\n<br />"
    "<table><tr><th align='left'>SoftAP config</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(softAP_ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.softAPIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>конфигрурация WLAN</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN список </th></tr>"
  );
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      server.sendContent(String() + "\r\n<tr><td>" + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : " *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
    }
  } else {
    server.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
  }
  server.sendContent(
    "</table>"
    "\r\n<br /><form method='POST' class='form-signin' action='wifisave'><h4>Connect to network:</h4>"
    "<input type='text' class='form-signin' placeholder='сеть Wifi' name='n'/>"
    "<br /><input type='password' class='form-signin' placeholder='пароль' name='p'/>"
    "<br /><input type='submit' class='btn btn-lg btn-primary btn-block' value='Подключить/Отключить'/></form>"

    "<a class='btn btn-block btn-default' href='/'>вернуться в управление</a>"
    "</div></div></div></body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();

  WiFi.disconnect();
  //WiFi.mode(WIFI_OFF);
  //delay(1000);
  //ESP.reset();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}
void save_wifiList(String s, String p) {
  String WifiList = readCommonFiletoJson("wifilist");
  String saveJson;
  String saveJsonName = "[";
  String saveJsonPass = "[";
  saveJson = "{num:1,";
  // if (WifiList != "") {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(WifiList);
  if (!rootjs.success()) {
    Serial.println("parseObject() save_wifiList failed");
    //return;
  }
  char num = rootjs["num"];
  num = 10;
  saveJson = "{num:" + String(num, DEC);

  char last = rootjs.containsKey("last") ? rootjs["last"] : char(-1);
  last > 10 ? last = 0 : true;
  char free_Index = -1;
  for (unsigned char i = 0; i < num; i++) {
    String nameWifi = rootjs["name"][i];
    if ((nameWifi == "") || ((nameWifi == s))) {
      free_Index = i;
    }
  }
  if (free_Index == char(-1)) {
    last++;
    last > 10 ? last = 0 : true;
    saveJson += ",last:" + String(char(last), DEC);
    free_Index = char(last);
  } else {
    saveJson += ",last:" + String(last, DEC);
  }
  for (unsigned char i = 0; i < num; i++) {
    String nameWifi = rootjs["name"][i];
    String passWifi = rootjs["pass"][i];
    if (i == free_Index) {
      saveJsonName += s;
      saveJsonPass += p;
    } else {
      saveJsonName += nameWifi;
      saveJsonPass += passWifi;
    }

    saveJsonName += i < num - 1 ? "," : "]";
    saveJsonPass += i < num - 1 ? "," : "]";
  }
  saveJson += ",name:" + saveJsonName + ",pass:" + saveJsonPass + "}";
  if (free_Index != char(-1)) {
    saveCommonFiletoJson("wifilist", saveJson);
  }
  saveCredentials();
  // }
  //else {
  //  saveCommonFiletoJson("wifilist", "{num:1,last:0,name:[" + String(s) + "],pass:[" + String(p) + "]}");
  //}
}
String *read_wifiList(unsigned char index) {
  String WifiList = readCommonFiletoJson("wifilist");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(WifiList);
  if (!rootjs.success()) {
    Serial.println("parseObject() save_wifiList failed");
    //return;
  }
  //char num = rootjs.containsKey("num") ? rootjs["num"] : 10;
  static String wifi[2];
  wifi[0] = rootjs["name"][index].as<String>();
  wifi[1] = rootjs["pass"][index].as<String>();
  return wifi;
}
void load_ssid_pass() {
  /*
    WiFi.disconnect();
    if (WiFi.getMode() != WIFI_AP)
    {
    Serial.println("Enable Wifi STA");
    WiFi.mode(WIFI_STA);
    delay(1000);
    }
  */
  Serial.println("scan start");
  char n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (char(n) > 0) {
    String WifiList = readCommonFiletoJson("wifilist");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& rootjs = jsonBuffer.parseObject(WifiList);
    if (!rootjs.success()) {
      Serial.println("parseObject() save_wifiList failed");
      return;
    }
    for (char i = 0; i < 10; i++) {
      String nameWifi = rootjs["name"][i];
      String passWifi = rootjs["pass"][i];
      //Serial.println(nameWifi);
      for (char in = 0; in < char(n); in++) {
        //Serial.println(WiFi.SSID((in)));
        if (String(WiFi.SSID(in)) == nameWifi) {
          nameWifi.toCharArray(ssid, sizeof(ssid) - 1);
          passWifi.toCharArray(password, sizeof(password) - 1);
          Serial.println(ssid);
          Serial.println(password);
          break;
        }
      }
    }
  }
}
void handleNotFound() {
  /*
    if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
    }
    handleFileRead("/home.htm");
  */
}
/** Is this an IP? */
boolean isIp(String str) {
  for (int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}


