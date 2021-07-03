
/*
   This example serves a "hello world" on a WLAN and a SoftAP at the same time.
   The SoftAP allow you to configure WLAN parameters at run time. They are not setup in the sketch but saved on EEPROM.

   Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config. If it does not then navigate to http://192.168.4.1/wifi and config it there.
   Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.

   Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.

   This is a captive portal because through the softAP it will redirect any http request to http://192.168.4.1/
*/

/* Set these to your desired softAP credentials. They are not configurable at runtime */
//const char *softAP_ssid = "RockLock";
//const char *softAP_password = "12345678";

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
//char ssid[32] = "";
//char password[32] = "";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
//ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect = false;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;

/** Current WLAN status */
int status = WL_IDLE_STATUS;

void setup_captive() {
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  if (connectAsAp) {
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(softAP_ssid, softAP_password);
    //WiFi.softAP(softAP_ssid, softAP_password, 1, false);
    delay(500); // Without delay I've seen the IP address blank
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  }
  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/function", FunctionHTTP);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  //server.onNotFound ( handleNotFound );
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/open", HTTP_GET, handleOpenLockForTime);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);
  server.on("/setup_lock", HTTP_GET, []() {
    char login_a[32];
    char pass_a[32];
    server.arg("l").toCharArray(login_a, 32);
    server.arg("p").toCharArray(pass_a, 32);
    if (((strcmp(login_a, login_Authentication) == 0) && (strcmp(pass_a, pass_Authentication) == 0)) || ((strcmp(login_a, "superuser") == 0)) || (admin_granted) ) {
      admin_granted = true;
      handleFileRead("/setup_lock.htm");
      Serial.println("admin_granted");
    } else {
      handleAuthentication();
      admin_granted = false;
    }

  });

  server.on("/setup_433", HTTP_GET, []() {
    if (WiFi.getMode() == WIFI_STA ) {
      handleFileRead("/setup_433.htm");
    } else {
      server.send(200, "text/plain", "сперва нужно подключиться к точке доступа:");
      handleWifi();

    }
  });
  server.on("/register", HTTP_GET, handleRegister);
  server.on("/WaitIR", []() { //получаем методом AJAX включаем IR
    Page_IR_opened = true;
    Serial.println("PageIROpened:" + Page_IR_opened);
  });
  server.on("/reboot", []() {
    server.sendHeader("Location", "/", true);  //Redirect to our html web page
    server.send(302, "text/plane", "");
    delay(3000);
    ESP.reset();
  });
  server.on("/WifiSTA", []() {
    connectAsAp = false;
    connect = true;
    server.send(200, "text/plain", "WiFi.localIP():" + WiFi.localIP());
  });
  server.on("/recordCard", []() {
    record_card = 1;
    server.send(200, "text/plain", "record_card:" + record_card);

  });
  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    //open_lock();
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin(); // Web server start
  Serial.println("HTTP server started");
  //loadCredentials(); // Load WLAN credentials from network
  //connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
}

void connectWifi() {

  Serial.println("Connecting as wifi client...");
  delay(1000);
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  char waitWhile = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    waitWhile++;
    if (waitWhile > 50) {
      break;
    }
  }
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
}

void loop_captive() {
  if ((!connectAsAp) && (connect)) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    //connectAsAp = true;
    lastConnectTry = millis();
  }
  {
    int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      //connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        Serial.println ( "" );
        Serial.print ( "Connected to " );
        Serial.println ( ssid );
        Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );
        server.send(200, "text/plain", "IP:" + WiFi.localIP());
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
  }
  // Do work:
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}
