 /** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);


  
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.




  server.sendContent(
    "<html><head></head><body>"
    "<meta name='viewport' http-equiv='Content-Type' content='width=device-width, initial-scale=1; charset=utf-8'/>"
    "<link rel='stylesheet' href='/bootstrap.min.css'>"
   "<link rel='stylesheet' type='text/css' href='/style.css'>"
     "<div class='container'>"
   "<div class='row' style='text-align:center;'>"
    "<h1 style='margin:50px;'>Настройка роутера</h1>"
    "<div class='col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6'>"
  );

  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>Вы подключены без точки доступа, напрямую: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>Вы подключены к сети: ") + ssid + "</p>");
  }
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
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname)+".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  
  server.sendContent(
    "<html><head></head><body>"
    "<meta name='viewport' http-equiv='Content-Type' content='width=device-width, initial-scale=1; charset=utf-8'/>"
    "<link rel='stylesheet' href='/bootstrap.min.css'>"
   "<link rel='stylesheet' type='text/css' href='/style.css'>"
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
      server.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
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
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
handleFileRead("/home.htm");
}


