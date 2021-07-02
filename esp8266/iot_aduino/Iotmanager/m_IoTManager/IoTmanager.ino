/*
 IoT Manager mqtt device client https://play.google.com/store/apps/details?id=ru.esp8266.iotmanager
 Based on Basic MQTT example with Authentication
 PubSubClient library v 1.91.1 https://github.com/Imroy/pubsubclient
  - connects to an MQTT server, providing userdescr and password
  - publishes config to the topic "/IoTmanager/config/deviceID/"
  - subscribes to the topic "/IoTmanager/hello" ("hello" messages from mobile device)

  Tested with Arduino IDE 1.6.6 + ESP8266 Community Edition v 2.0.0-stable and PubSubClient library v 1.91.1 https://github.com/Imroy/pubsubclient
  ESP8266 Community Edition v 2.0.0-stable have some HTTPS issues. Push notification temporary disabled.

  sketch version : 1.5
  IoT Manager    : any version

  toggle, range, small-badge widgets demo
*/


//const char *ssid =  "Home";            // cannot be longer than 32 characters!
//const char *pass =  "sp5506487";       // WiFi password



WiFiClient wclient;

// config for cloud mqtt broker by DNS hostname ( for example, cloudmqtt.com use: m20.cloudmqtt.com - EU, m11.cloudmqtt.com - USA )
//String mqttServerName = "m20.cloudmqtt.com";            // for cloud broker - by hostname, from CloudMQTT account data
//unsigned int    mqttport = 16238;                                // default 1883, but CloudMQTT.com use other, for example: 13191, 23191 (SSL), 33191 (WebSockets) - use from CloudMQTT account data
//String mqttuser =  "spspider";                              // from CloudMQTT account data
//String mqttpass =  "5506487";                              // from CloudMQTT account data
PubSubClient client(wclient, mqttServerName, mqttport); // for cloud broker - by hostname


// config for local mqtt broker by IP address
//IPAddress server(192, 168, 1, 100);                        // for local broker - by address
//int    mqttport = 1883;                                    // default 1883
//String mqttuser =  "";                                     // from broker config
//String mqttpass =  "";                                     // from broker config
//PubSubClient client(wclient, server, mqttport);            // for local broker - by address

String val;
String ids = "";
unsigned int oldtime, newtime, pushtime,newtimePWM,oldtimePWM;
int newValue;
char client_iot_busy = 0;
const String stat1 = "{\"status\":\"1\"}";
const String stat0 = "{\"status\":\"0\"}";

unsigned int nWidgets = 8;
const unsigned int nWidgetsArray = 10;
String stat        [nWidgetsArray];
String sTopic      [nWidgetsArray];
//String color       [nWidgetsArray];
//String style       [nWidgetsArray];
//String badge       [nWidgetsArray];
String widget      [nWidgetsArray];
String descr       [nWidgetsArray];
String page        [nWidgetsArray];
String thing_config[nWidgetsArray];
String id          [nWidgetsArray];
int    pin         [nWidgetsArray];
int    defaultVal  [nWidgetsArray];
unsigned int low_pwm[nWidgetsArray];
bool low_pwm_off=false;//low_pwm
String   pinmode   [nWidgetsArray];

// Push notifications
const char* host = "onesignal.com";
WiFiClientSecure httpClient;
const int httpsPort = 443;
String url = "/api/v1/notifications";

void push(String msg) {
  Serial.println("PUSH: try to send push notification...");
  if (ids.length() == 0) {
    Serial.println("PUSH: ids not received, push failed");
    return;
  }
  if (!httpClient.connect(host, httpsPort)) {
    Serial.println("PUSH: connection failed");
    return;
  }
  String data = "{\"app_id\": \"8871958c-5f52-11e5-8f7a-c36f5770ade9\",\"include_player_ids\":[\"" + ids + "\"],\"android_group\":\"IoT Manager\",\"contents\": {\"en\": \"" + msg + "\"}}";
  httpClient.println("POST " + url + " HTTP/1.1");
  httpClient.print("Host:");
  httpClient.println(host);
  httpClient.println("User-Agent: esp8266.Arduino.IoTmanager");
  httpClient.print("Content-Length: ");
  httpClient.println(data.length());
  httpClient.println("Content-Type: application/json");
  httpClient.println("Connection: close");
  httpClient.println();
  httpClient.println(data);
  httpClient.println();
  Serial.println(data);
  Serial.println("PUSH: done.");
}
String setStatus ( String s ) {
  String stat = "{\"status\":\"" + s + "\"}";
  return stat;
}
String setStatus ( int s ) {
  String stat = "{\"status\":\"" + String(s) + "\"}";
  return stat;
}

void initVar() {
  pinmode[0] = "out";
  id    [0] = "0";
  page  [0] = "Kitchen";
  descr [0] = "Light-0";
  widget[0] = "toggle";
  pin[0] = 4;                                              // GPIO4 - toggle
  defaultVal[0] = 0;                                       // defaultVal status

  //  sTopic[0]   = prefix + "/" + deviceID + "/" + descr[0] + id[0];
  //  color[0]   = "\"color\":\"blue\"";                       // black, blue, green, orange, red, white, yellow (off - grey)

  pinmode[1] = "out";
  id    [1] = "1";
  page  [1] = "Kitchen";
  descr [1] = "Light-1";
  widget[1] = "toggle";
  pin[1] = 5;                                              // GPIO5 - toggle
  defaultVal[1] = 0;                                       // defaultVal status

  // sTopic[1]   = prefix + "/" + deviceID + "/" + descr[1] + id[1];
  //  color [1] = "\"color\":\"orange\"";                       // black, blue, green, orange, red, white, yellow (off - grey)

  pinmode[2] = "pwm";
  id    [2] = "2";
  page  [2] = "Kitchen";
  descr [2] = "Dimmer";
  widget[2] = "range";
  pin[2] = 0;                                              // GPIO0 - range
  defaultVal[2] = 1023;                                    // defaultVal status 0%, inverted

  //  sTopic[2]   = prefix + "/" + deviceID + "/" + descr[2] + id[2];
  //  style[2]   = "\"style\":\"range-calm\"";                 // see http://ionicframework.com/docs/components/#colors
  //  badge[2]   = "\"badge\":\"badge-assertive\"";            // see http://ionicframework.com/docs/components/#colors

  pinmode[3] = "adc";
  id    [3] = "3";
  page  [3] = "Kitchen";
  descr  [3] = "ADC";
  widget[3] = "small-badge";
  pin   [3] = A0;                                          // ADC
  // sTopic[3]   = prefix + "/" + deviceID + "/" + descr[3] + id[3];
  //  badge [3] = "\"badge\":\"badge-balanced\"";             // see http://ionicframework.com/docs/components/#colors

  pinmode[4] = "in";
  id    [4] = "4";
  page  [4] = "Outdoor";
  descr [4] = "Garden light";
  widget[4] = "toggle";
  pin   [4] = 2;                                           // GPIO2
  defaultVal[4] = 1;                                       // defaultVal status 0%

  //  sTopic[4]   = prefix + "/" + deviceID + "/" + descr[4] + id[4];
  //  color [4] = "\"color\":\"red\"";                         // black, blue, green, orange, red, white, yellow (off - grey)

  // RED
  pinmode[5] = "pwm";
  id    [5] = "5";
  page  [5] = "Kitchen";
  descr [5] = "RED";
  widget[5] = "range";
  pin   [5] = 15;                                          // GPIO15 - range
  defaultVal[5] = 0;                                       // defaultVal 0%, not inverted
  //  sTopic[5]   = prefix + "/" + deviceID + "/" + descr[5] + id[5];
  //  style[5]   = "\"style\":\"range-assertive\"";            // see http://ionicframework.com/docs/components/#colors
  //  badge[5]   = "\"badge\":\"badge-assertive\"";            // see http://ionicframework.com/docs/components/#colors

  // GREEN
  pinmode[6] = "pwm";
  id    [6] = "6";
  page  [6] = "Kitchen";
  descr [6] = "GREEN";
  widget[6] = "range";
  pin[6] = 12;                                      // GPIO12 - range
  defaultVal[6] = 0;                                       // defaultVal 0%
  //  sTopic[6]   = prefix + "/" + deviceID + "/" + descr[6] + id[6];
  //  style[6]   = "\"style\":\"range-balanced\"";         // see http://ionicframework.com/docs/components/#colors
  //  badge[6]   = "\"badge\":\"badge-balanced\"";          // see http://ionicframework.com/docs/components/#colors

  // BLUE
  pinmode[7] = "pwm";
  id    [7] = "7";
  page  [7] = "Kitchen";
  descr [7] = "BLUE";
  widget[7] = "range";
  pin[7] = 13;                                      // GPIO13 - range
  defaultVal[7] = 0;                                       // defaultVal status 0%
  //  sTopic[7]   = prefix + "/" + deviceID + "/" + descr[7] + id[7];
  // style[7]   = "\"style\":\"range-calm\"";             // see http://ionicframework.com/docs/components/#colors
  // badge[7]   = "\"badge\":\"badge-calm\"";              // see http://ionicframework.com/docs/components/#colors
  /////////////////////////////////////

  ////////////////////////////////////////


  initThingConfig();
}
void initThingConfig() {
  for (int i = 0; i < nWidgets; i++) {
    sTopic[i]   = prefix + "/" + deviceID + "/" + descr[i] + id[i];
  }
  unsigned int *value_back = getSPIFFS_JSON_VALUE(nWidgets);
  for (int i = 0; i < nWidgets; i++) {
    defaultVal[i] = value_back[i];
  }
  for (int i = 0; i < nWidgets; i++) {
    stat[i] = setStatus(defaultVal[i]);
    thing_config[i] = "{\"id\":\"" + id[i] + "\",\"page\":\"" + page[i] + "\",\"descr\":\"" + descr[i] + "\",\"widget\":\"" + widget[i] + "\",\"topic\":\"" + sTopic[i] + "\"}"; // GPIO switched On/Off by mobile widget toggle
  }
  /*
  thing_config[0] = "{\"id\":\"" + id[0] + "\",\"page\":\"" + page[0] + "\",\"descr\":\"" + descr[0] + "\",\"widget\":\"" + widget[0] + "\",\"topic\":\"" + sTopic[0] + "\"," + color[0] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[1] = "{\"id\":\"" + id[1] + "\",\"page\":\"" + page[1] + "\",\"descr\":\"" + descr[1] + "\",\"widget\":\"" + widget[1] + "\",\"topic\":\"" + sTopic[1] + "\"," + color[1] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[2] = "{\"id\":\"" + id[2] + "\",\"page\":\"" + page[2] + "\",\"descr\":\"" + descr[2] + "\",\"widget\":\"" + widget[2] + "\",\"topic\":\"" + sTopic[2] + "\"," + style[2] + "," + badge[2] + ",\"leftIcon\":\"ion-ios-rainy-outline\",\"rightIcon\":\"ion-ios-rainy\"}"; // for icons see http://ionicons.com
  thing_config[3] = "{\"id\":\"" + id[3] + "\",\"page\":\"" + page[3] + "\",\"descr\":\"" + descr[3] + "\",\"widget\":\"" + widget[3] + "\",\"topic\":\"" + sTopic[3] + "\"," + badge[3] + "}"; // ADC
  thing_config[4] = "{\"id\":\"" + id[4] + "\",\"page\":\"" + page[4] + "\",\"descr\":\"" + descr[4] + "\",\"widget\":\"" + widget[4] + "\",\"topic\":\"" + sTopic[4] + "\"," + color[4] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[5] = "{\"id\":\"" + id[5] + "\",\"page\":\"" + page[5] + "\",\"descr\":\"" + descr[5] + "\",\"widget\":\"" + widget[5] + "\",\"topic\":\"" + sTopic[5] + "\"," + style[5] + "," + badge[5] + "}"; // GPIO15 R
  thing_config[6] = "{\"id\":\"" + id[6] + "\",\"page\":\"" + page[6] + "\",\"descr\":\"" + descr[6] + "\",\"widget\":\"" + widget[6] + "\",\"topic\":\"" + sTopic[6] + "\"," + style[6] + "," + badge[6] + "}"; // GPIO12 G
  thing_config[7] = "{\"id\":\"" + id[7] + "\",\"page\":\"" + page[7] + "\",\"descr\":\"" + descr[7] + "\",\"widget\":\"" + widget[7] + "\",\"topic\":\"" + sTopic[7] + "\"," + style[7] + "," + badge[7] + "}"; // GPIO13 B
  */
}
// send confirmation
void pubStatus(String t, String payload) {
  if (client.publish(t + "/status", payload)) {
    Serial.println("Publish new status for " + t + ", value: " + payload);
  } else {
    Serial.println("Publish new status for " + t + " FAIL!");
  }
}
void pubConfig() {
  bool success;
  success = client.publish(MQTT::Publish(prefix, deviceID).set_qos(1));
  if (success) {
    delay(500);
    for (int i = 0; i < nWidgets; i = i + 1) {
      success = client.publish(MQTT::Publish(prefix + "/" + deviceID + "/config", thing_config[i]).set_qos(1));
      if (success) {
        Serial.println("Publish config: Success (" + thing_config[i] + ")");
      } else {
        Serial.println("Publish config FAIL! ("    + thing_config[i] + ")");
      }
      delay(150);
    }
  }
  if (success) {
    Serial.println("Publish config: Success");
  } else {
    Serial.println("Publish config: FAIL");
  }
  for (int i = 0; i < nWidgets; i = i + 1) {
    pubStatus(sTopic[i], stat[i]);
    delay(150);
  }
}


void callback(const MQTT::Publish& sub) {
  Serial.print("Get data from subscribed topic ");
  Serial.print(sub.topic());
  Serial.print(" => ");
  Serial.println(sub.payload_string());


  for (int i = 0; i < nWidgets; i++) {
    if (sub.topic() == sTopic[i] + "/control") {
      if ((pinmode[i] == "out") || (pinmode[i] == "in")) {
        if (sub.payload_string() == "0") {
          newValue = 0; // inverted
          stat[i] = stat0;
        } else {
          newValue = 1;
          stat[i] = stat1;
        }
        digitalWrite(pin[i], newValue);
        pubStatus(sTopic[i], stat[i]);
      } else if (pinmode[i] == "pwm") {
        String x = sub.payload_string();
        analogWrite(pin[i], 1023 - x.toInt());
        stat[i] = setStatus(x);
        pubStatus(sTopic[i], stat[i]);
      }
      else if (pinmode[i] == "adc") {
      }
      break;
    }
  }

  /*
  if (sub.topic() == sTopic[0] + "/control") {
    if (sub.payload_string() == "0") {
      newValue = 0; // inverted
      stat[0] = stat0;
    } else {
      newValue = 1;
      stat[0] = stat1;
    }
    digitalWrite(pin[0], newValue);
    pubStatus(sTopic[0], stat[0]);
  } else if (sub.topic() == sTopic[1] + "/control") {
    if (sub.payload_string() == "0") {
      newValue = 0; // inverted
      stat[1] = stat0;
    } else {
      newValue = 1; // inverted
      stat[1] = stat1;
    }
    digitalWrite(pin[1], newValue);
    pubStatus(sTopic[1], stat[1]);
  } else if (sub.topic() == sTopic[2] + "/control") {
    String x = sub.payload_string();
    analogWrite(pin[2], 1023 - x.toInt());
    stat[2] = setStatus(x);
    pubStatus(sTopic[2], stat[2]);
  } else if (sub.topic() == sTopic[3] + "/control") {
    // ADC : nothing, display only
  } else if (sub.topic() == sTopic[4] + "/control") {
    if (sub.payload_string() == "0") {
      newValue = 0; // inverted
      stat[4] = stat0;
    } else {
      newValue = 1; // inverted
      stat[4] = stat1;
    }
    digitalWrite(pin[4], newValue);
    pubStatus(sTopic[4], stat[4]);
  } else if (sub.topic() == sTopic[5] + "/control") {
    String x = sub.payload_string();
    analogWrite(pin[5], x.toInt());
    stat[5] = setStatus(x);
    pubStatus(sTopic[5], stat[5]);
  } else if (sub.topic() == sTopic[6] + "/control") {
    String x = sub.payload_string();
    analogWrite(pin[6], x.toInt());
    stat[6] = setStatus(x);
    pubStatus(sTopic[6], stat[6]);
  } else if (sub.topic() == sTopic[7] + "/control") {
    String x = sub.payload_string();
    analogWrite(pin[7], x.toInt());
    stat[7] = setStatus(x);
    pubStatus(sTopic[7], stat[7]);
  } else
  */
  if (sub.topic() == prefix + "/ids") {
    ids = sub.payload_string();
  } else if (sub.topic() == prefix) {
    if (sub.payload_string() == "HELLO") {
      pubConfig();
    }
  }
}
void Setup_pinmode() {
  for (int i = 0; i < nWidgets; i++) {
    if (pinmode[i] == "in") {
      pinMode(pin[i], INPUT);
      stat[i] = setStatus(digitalRead(pin[i]));
      Serial.println("set input:" + String(pin[i]) + "i:" + i);
    }
    if (pinmode[i] == "out") {
      pinMode(pin[i], OUTPUT);
      digitalWrite(pin[i], defaultVal[i]);
      stat[i] = setStatus(defaultVal[i]);
      Serial.println("set output:" + String(pin[i]) + "i:" + i + "defaultVal:" + defaultVal[i]);
    }
    if (pinmode[i] == "pwm") {
      pinMode(pin[i], OUTPUT);
      analogWrite(pin[i], defaultVal[i]); // PWM
      Serial.println("set pwm:" + String(pin[i]) + "i:" + String(i) + "default:" + String(defaultVal[i]));
    }
    if (pinmode[i] == "low_pwm") {
      pinMode(pin[i], OUTPUT);
      low_pwm[i] = defaultVal[i];
      digitalWrite(pin[i], 1);//далее - выключаем
      Serial.println("set low_pwm:" + String(pin[i]) + "i:" + String(i) + "default:" + String(defaultVal[i]));
    }
    if (pinmode[i] == "adc") {
      stat[i] = setStatus(analogRead(pin[i]));//adc pin:A0
      Serial.println("read adc:" + String(pin[i]) + "i:" + String(i) + "stat:" + String( stat[i]));
    }
  }
}
void setup_IOTManager() {


if (loadWidgets()) {
  Serial.println("widgets Loaded");
}
else {
  initVar();
}

Setup_pinmode();
/*
pinMode(pin[0], OUTPUT);
digitalWrite(pin[0], defaultVal[0]);

pinMode(pin[1], OUTPUT);
digitalWrite(pin[1], defaultVal[1]);

pinMode(pin[2], OUTPUT);
digitalWrite(pin[2], defaultVal[2]);

stat[3] = setStatus(analogRead(pin[3]));//adc pin:A0

pinMode(pin[4], INPUT);
stat[4] = setStatus(digitalRead(pin[4]));

pinMode(pin[5], OUTPUT);
analogWrite(pin[5], defaultVal[5]); // PWM

pinMode(pin[6], OUTPUT);
analogWrite(pin[6], defaultVal[6]); // PWM

pinMode(pin[7], OUTPUT);
analogWrite(pin[7], defaultVal[7]); // PWM
*/
// Setup console

//Serial.println("MQTT client started.");
//Serial.print("Free heap = ");
//Serial.println(ESP.getFreeHeap());
}

void loop_IOTMAnager() {

  /*
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting via WiFi to ");
    Serial.print(ssid);
    Serial.println("...");

    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      return;
    }

    Serial.println("");
    Serial.println("WiFi connect: Success");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  */
  //Serial.println("WifiStatus:"+WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {

      Serial.println("Connecting to MQTT server ...");
      bool success = false;
      if (internet) {
        if (mqttuser.length() > 0) {
          success = client.connect( MQTT::Connect( deviceID ).set_auth(mqttuser, mqttpass) );
        } else {
          success = client.connect( deviceID );
        }
      }
      if (success) {
        no_internet_timer = millis();
        client.set_callback(callback);
        Serial.println("Connect to MQTT server: Success");
        pubConfig();
        client.subscribe(prefix);                 // for receiving HELLO messages
        client.subscribe(prefix + "/ids");        // for receiving IDS  messages
        for (int i = 0; i < nWidgets; i++) {
          client.subscribe(sTopic[i] + "/control"); // for receiving GPIO messages
        }
        /*
                client.subscribe(sTopic[0] + "/control"); // for receiving GPIO messages
                client.subscribe(sTopic[1] + "/control"); // for receiving GPIO messages
                client.subscribe(sTopic[2] + "/control"); // for receiving GPIO messages
                // 3 - display only, no control
                client.subscribe(sTopic[4] + "/control"); // for receiving GPIO messages
                client.subscribe(sTopic[5] + "/control"); // for receiving GPIO messages
                client.subscribe(sTopic[6] + "/control"); // for receiving GPIO messages
                client.subscribe(sTopic[7] + "/control"); // for receiving GPIO messages
        */
        Serial.println("Subscribe: Success");
      } else {
        success = false;
        long wait_for_sec = ((no_internet_timer + 30000) - millis()) / 1000;
        Serial.print("Connect to MQTT server: FAIL wait:");
        Serial.print(wait_for_sec);
        Serial.println("sec");
        delay(1000);
        internet = false;
        success = false;

        //if 5 times fail - no internet connection
        try_internet_access = false;
      }
    }

    if (client.connected()) {
      newtime = millis();
      if (newtime - oldtime > 10000) { // 10 sec
        int x;
        for (int i = 0; i < nWidgets; i++) {
          if (pinmode[i] == "adc") {
            x = analogRead(pin[i]);
            val = "{\"status\":\"" + String(x) + "\"}";
            client.publish(sTopic[i] + "/status", val );  // widget 3
          }
        }

        oldtime = newtime;
        if ((millis() - pushtime > 10000) && (x > 100)) {
          String msg = "Bedroom ADC more then 100! (" + String(x) + ")";
          //
          // ESP8266 Community Edition v 2.0.0-stable have some HTTPS issues. Push notification temporary disables. Please, uncomment next line if you use future versions.
          //
          // push(msg);
          //
          //
          pushtime = millis();
        }
      }
      /*
      int key;
      key = digitalRead(pin[4]);
      if ( stat[4] != setStatus(key) ) {
        stat[4] = setStatus(key);
        pubStatus(sTopic[4], stat[4] );  // widget 4
      }
      */
      client.loop();
    }
  }
}
