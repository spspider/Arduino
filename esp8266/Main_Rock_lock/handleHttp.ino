/** Handle root or redirect to captive portal */
/** Is this an IP? */

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
  //open_lock();
  /*
    if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    // return;
    }
  */
  sendMyheader();

  String Page = sendHead();
  Page += F("<h1>ЗАМОК</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>Замок: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  if (!register_is_on) {
    if ((open_type == 0) || (open_type == 2)) {
      Page += F(
                "<p>открыть замок:</p>"
                "<p><br><a href='/open?t=255'><button style='font-size:40px' >открыть</button></a></p>"
              );
    }
    Page += F(
              "<p><br><a href='/setup_lock'><button>setup</button></a></p>"
              "</body></html>");
    /*
                   "<p><br><a href='/open?t=10'><button>открыть на 10 минут</button></a></p>"
                "<p><br><a href='/open?t=0'><button>закрыть</button></a></p>"
    */
  } else {
    /*
      String mac_adress = readCommonFiletoJson("mac_adress");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& rootjs = jsonBuffer.parseObject(mac_adress);
      if (!rootjs.success()) {
      Serial.println("FAIL mac_adress!!!!");
      }
      uint8_t num = rootjs["num"];
      bool found_compare;
      num > 254 ? num = 254 : 0;
      for (uint8_t i = 0; i < num; i++) {
      char mac[32];
      if (rootjs["mac"][i]) {
        strcpy(mac, rootjs["mac"][i]);
        if (strcmp(mac, str_connected_mac) == 0) {
          found_compare = true;
          Page += F("<p>mac:");
          Page += String(mac);
          Page += F(" <mark>added!</mark></p>");
          Page += F("<p>открыть замок:</p>"
                    "<p><br><a href='/open?t=255'><button style='font-size:40px' >открыть</button></a></p>");
        }
      }
      }
    */
    if (compare_found_global) {
      Page += F("<p>mac:");
      Page += String(str_connected_mac);
      Page += F(" <mark>added!</mark></p>");
      Page += F("<p>открыть замок:</p>"
                "<p><br><a href='/open?t=255'><button style='font-size:40px' >открыть</button></a></p>");
    }
    else {
      Page += F("<p>ERROR mac not added</p>");
    }

    Page += F("</body></html>");
  }
  server.send(200, "text/html", Page);
}
void handleRegister() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }

  //server.send(200, "text/plain", "сперва нужно подключиться к точке доступа:");
  //handleWifi();
  uint8_t sw_reg = server.arg("reg").toInt();

  sendMyheader();
  String Page = sendHead();
  switch (sw_reg) {
    case 0://show page
      {
        Page += F("<h1>REGISTER</h1>");
        break;
      }
    case 1: //registraion is on
      register_is_on = true;
      updateSettings();
      //server.send(200, "text/plain", "registraion is on");
      Page += F("<h1>registraion is on</h1>");
      break;

    case 2: //registraion is off
      register_is_on = false;
      updateSettings();
      //server.send(200, "text/plain", "registraion is off");
      Page += F("<h1>registraion is off</h1>");
      break;
    case 3: //registraion is off
      //register_is_on = false;
      Page += F("<h1>erasing complete</h1>");
      //      server.send(200, "text/plain", "erasing complete");
      break;

  }
  Page += F(

            "<p><br><a href='/register?reg=1'><button style='font-size:40px' >on</button></a></p>"
            "<p><br><a href='/register?reg=2'><button style='font-size:40px' >off</button></a></p>"
            "<p><br><a href='/setup_lock'><button style='font-size:40px' >setup lock</button></a></p>"
            "<p><br><a href='/'><button style='font-size:40px' >control</button></a></p>"
            "<p><br><br><br><a href='/register?reg=3'><button style='font-size:40px' >Erase ALL</button></a></p>"

            "</body></html>");
  server.send(200, "text/html", Page);


}
void handleAuthentication() {

  sendMyheader();
  String Page = sendHead();
  Page += F("<h1>Authentication</h1>"
            "\r\n<br /><form method='GET' action='setup_lock'>"
            "<table style=\"font-size: 40px;\" width=\"73\">\n"
            "<tbody>\n"
            "<tr>\n"
            "<td style=\"width: 67px;\">login</td>\n"
            "<td style=\"width: 0px;\"><input type='text' name='l'/></td>\n"
            "</tr>\n"
            "<tr>\n"
            "<td style=\"width: 67px;\">password</td>\n"
            "<td style=\"width: 0px;\"><input type='text' name='p'/></td>\n"
            "</tr>\n"
            "</tbody>\n"
            "</table>\n"
            "<br /><input type='submit' style=\"font-size: 40px;\" value='ok'/></form>"
            "</body></html>");
  server.send(200, "text/html", Page);
}
void FunctionHTTP() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
  if (!root.success()) {
    Serial.println("FAIL FUNCTION!!!!" + server.arg("json"));
    return;
  }
  if (root.containsKey("Authentication")) {
    uint8_t key = root["Authentication"];
    switch (key) {
      case 0:
        //        const char buffer[10]=[admin_granted];
        //strcpy(buffer, admin_granted)
        server.send(200, "text/plane", String(admin_granted));
        break;
      case 1:
        handleAuthentication();
        break;
    }
  }

}
/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    station_connected = true;
    //open_lock();
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  sendMyheader();
  String Page = sendHead();
  Page += F("<h1>Wifi config</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  Page +=
    String(F(
             "\r\n<br />"
             "<table><tr><th align='left'>SoftAP config</th></tr>"
             "<tr><td>SSID ")) +
    String(softAP_ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.softAPIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN config</th></tr>"
      "<tr><td>SSID ") +
    String(ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.localIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  } else {
    Page += F("<tr><td>No WLAN found</td></tr>");
  }
  Page += F(
            "</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n'/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>"
            "<p><br><a href='/'><button>главная</button></a></p>"
            "<p><br><a href='/setup_lock'><button>настройки замка</button></a></p>"
            "<p><br><a href='/setup_433'><button>записать пульт</button></a></p>"
            "</body></html>");
  server.send(200, "text/html", Page);
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
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  //saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}


