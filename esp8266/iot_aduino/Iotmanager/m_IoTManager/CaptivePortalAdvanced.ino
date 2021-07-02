

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
/*
char *softAP_ssid = "ESP_ap";
char *softAP_password = "12345678";
char ssid[32] = "";
char password[32] = "";

*/
// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

unsigned char softap_stations_cnt;
struct station_info *stat_info;
struct ip_addr *IPaddress;
uint32 uintaddress;
// Web server
//ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;
long lastConnectTry_wrongPassword = 0;
/** Current WLAN status */
int status = WL_IDLE_STATUS;

void connect_as_AccessPoint() {
  delay(1000);

  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay( 500 ); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());


}
void captive_setup() {


  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/setup", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound ( handleNotFound );
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  loadCredentials(); // Load WLAN credentials from network
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
  if (connect) {

    try_internet_access = true;
  }
  else {

    try_internet_access = false;
    connect_as_AccessPoint();
  }
}

void connectWifi() {
  if (WiFi.getMode() != WIFI_STA)
  {
    Serial.println("Enable Wifi STA");
    WiFi.mode(WIFI_STA);
    delay(1000);
  }
  Serial.println("Connecting as wifi client...");
  
  //WiFi.disconnect();
  delay(100);
  WiFi.begin ( ssid, password );
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
}
void captive_loop() {
  if (connect) {//если есть SSID в EEPROM
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }

  /*
  if(try_internet_access){  loop_IOTMAnager();}
  if ((!try_internet_access)&&(WiFi.status() == WL_CONNECTED)&&(WiFi.getMode() == WIFI_STA)){
             if (millis() > no_internet_timer + 30000 ) {//пробовать подключится каждые30 сек
            internet=true;//пробуем подключить MQTT
            try_internet_access=true;
            Serial.println("Connect again");
            no_internet_timer=millis();
           }
    }
  */
  /*
  if (!internet) {
    if (millis() > no_internet_timer + 30000 ) {
      connect_as_AccessPoint();
      int n = WiFi.scanNetworks();
      Serial.println("scan done");
      if (n > 0) {
        int a;
        for (int i = 0; i < n; i++) {
          if (WiFi.encryptionType(i) == ENC_TYPE_NONE)  {
            for (int i1 = 0; i1 < scannedFREEWIFI_False.length; i1++) {
              if (WiFi.SSID(a) != scannedFREEWIFI_False[i1]) {
                scannedFREEWIFI[a] = WiFi.SSID(a);
              }
            }

            a++;
          }

        }
      }
      no_internet_timer = millis();
    }
  }
  */
  {
    int s = WiFi.status();
    if ((s == 0) || (s == 6) && millis() > lastConnectTry + 30000 ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */

      if (WiFi.getMode() != WIFI_AP) {
        Serial.println ( "Wrong connection" );
        connect_as_AccessPoint();
      }
      else if ((WiFi.getMode() == WIFI_AP) && (strlen(ssid) > 0) && (WiFi.softAPgetStationNum() == 0)) {
        connect = true;
      }
      lastConnectTry = millis();
    }

    //else if (((millis()-TrytoConnectAgainTime)>60000)&&(strlen(ssid)>0)&&(WiFi.getMode() == WIFI_AP)){//подключить Wifi клиент снова
    //lastConnectTry=0;
    // TrytoConnectAgainTime=millis();

    //  }
    //else if (WiFi.getMode()==WIFI_AP)

    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );
      status = s;
      if (s == WL_CONNECTED) {
        try_internet_access = true;//можно попытаться подключиться к интернету
        /* Just connected to WLAN */
        Serial.println ( "" );
        Serial.print ( "Connected to " );
        Serial.println ( ssid );
        Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);

          Serial.println("Start 8-Time");
          setup_time();
          updatetime();
        }

      } else if (s == WL_NO_SSID_AVAIL) {
        try_internet_access = false;
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
