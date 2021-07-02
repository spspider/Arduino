/** Handle root or redirect to captive portal */
void sendMyheader() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
}
String sendHead() {
  String Page_head;
  Page_head += F(
                 "<html><head>"
                 "<meta http-equiv='Content-type' content='text/html; charset=utf-8'>"
                 "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                 "</head><body>"
               );
  return Page_head;
}
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
   // return;
  }
  sendMyheader();

  String Page = sendHead();
  Page += F("<h1>DEV</h1>");
  /*
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  */
  Page += F(
                        "<p><br><a href='/home.htm'><button>домашняя страница</button></a></p>"
            "<p><br><a href='/wifi'><button>подключение к Wifi</button></a></p>"
            "<p><br><a href='/other_setup'><button>другие настройки</button></a></p>"
            "<p><br><a href='/pin_setup'><button>pin setup</button></a></p>"
            "<p><br><a href='/IR_setup'><button>ИК настройки</button></a></p>"
            "<p><br><a href='/condition'><button>условия</button></a></p>"
            "<p><br><a href='/function?json={reboot:1}'><button>перезапуск</button></a></p>"
            "<p><br><br><a href='/open?t=1'><button>открыть замок</button></a></p>");


  server.send(200, "text / html", Page);
}
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    //server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.sendHeader("Location", String("http://"), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}
void handleWifilist() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = ssid;
  json["WiFilocalIP"] = toStringIp(WiFi.localIP());
  json["softAP"] = softAP_ssid;
  json["WiFisoftAPIP"] = toStringIp(WiFi.softAPIP());


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
}
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);

  saveCredentials();

  connectWifi(ssid, password);
}
void save_wifiList(String s, String p) {
  String WifiList = readCommonFiletoJson("wifilist");
  //WifiList==""?return:true;
  String saveJson;
  String saveJsonName = "[";
  String saveJsonPass = "[";
  saveJson = " {num:1, ";
  // if (WifiList != "") {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(WifiList);
  if (!rootjs.success()) {
    Serial.println("parseObject() save_wifiList failed");
    //return;
  }
  char num = rootjs["num"];
  num = 10;


/*
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["num"] = num;
  JsonArray& name_array = json.createNestedArray("name");
  JsonArray& pass_array = json.createNestedArray("pass");
  */
  
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
    saveJson += ", last:" + String(char(last), DEC);
    free_Index = char(last);
    } else {
    saveJson += ", last:" + String(last, DEC);
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

    saveJsonName += i < num - 1 ? ", " : "]";
    saveJsonPass += i < num - 1 ? ", " : "]";
    }
    saveJson += ", name : " + saveJsonName + ", pass : " + saveJsonPass + "}";
    if (free_Index != char(-1)) {
    saveCommonFiletoJson("wifilist", saveJson, 1);
    }
  
  saveCredentials();
  // }
  //else {
  //  saveCommonFiletoJson("wifilist", "{num : 1, last : 0, name : [" + String(s) + "], pass : [" + String(p) + "]}");
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
bool load_ssid_pass() {
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
      return false;
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
  return true;
}
void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI : ");
  message += server.uri();
  message += F("\nMethod : ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments : ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(" : ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache - Control", "no - cache, no - store, must - revalidate");
  server.sendHeader("Pragma", "no - cache");
  server.sendHeader("Expires", " - 1");
  server.send(404, "text / plain", message);
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


