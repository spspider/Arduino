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
PubSubClient client(wclient); // for cloud broker - by hostname


// config for local mqtt broker by IP address
//IPAddress server(192, 168, 1, 100);                        // for local broker - by address
//int    mqttport = 1883;                                    // default 1883
//String mqttuser =  "";                                     // from broker config
//String mqttpass =  "";                                     // from broker config
//PubSubClient client(wclient, server, mqttport);            // for local broker - by address

//String val;
//String ids = "";
unsigned int oldtime, newtime, newtimePWM, oldtimePWM;
//int newValue;
//char client_iot_busy = 0;
//const String stat1 = "{\"status\":\"1\"}";
//const String stat0 = "{\"status\":\"0\"}";

char nWidgets = 8;
const char nWidgetsArray = 10;
int stat        [nWidgetsArray];
String sTopic      [nWidgetsArray];
char widget      [nWidgetsArray];// inputWidjet[0] = 'unknown';1 = 'toggle';2 = 'simple-btn';4 = 'range';4 = 'small-badge';5 = 'chart';
String descr       [nWidgetsArray];
//String thing_config[nWidgetsArray];
char    id          [nWidgetsArray];
char    pin         [nWidgetsArray];
int    defaultVal  [nWidgetsArray];
char    IrButtonID  [nWidgetsArray];
float analogDivider = 1.0F;
int analogSubtracter = 0;
unsigned int low_pwm[nWidgetsArray];
bool low_pwm_off = false; //low_pwm
char   pinmode   [nWidgetsArray];//inputPinmode[0] = "no pin";inputPinmode[1] = "in";inputPinmode[2] = "out";inputPinmode[3] = "pwm";inputPinmode[4] = "adc";inputPinmode[5] = "low_pwm";inputPinmode[6] = "IR";inputPinmode[7] = "???????????? ???????? MQ7";

unsigned char subscribe_loop = 0;
unsigned long subscr_sec;
/*
  char stat[nWidgetsArray][200];
  char sTopic[nWidgetsArray][200];
  char widget[nWidgetsArray][200];
  char descr[nWidgetsArray][200];
  char page[nWidgetsArray][200];
  char thing_config[nWidgetsArray][200];
  char pinmode[nWidgetsArray][200];
  //char test8[nWidgetsArray][200];
*/
// Push notifications

String setStatus ( String s ) {
  String stat;
  if (type_mqtt == 0) {
    stat = "{\"status\":\"" + s + "\"}";
  }
  else if (type_mqtt == 1) {
    stat = s;
  }
  return stat;
}
String setStatus ( int s ) {
  String stat;
  if (type_mqtt == 0) {
    stat = "{\"status\":\"" + String(s) + "\"}";
  }
  if (type_mqtt == 1) {
    stat = String(s);
  }
  return stat;
}

void initVar() {


  initThingConfig();
}
void initThingConfig() {
  for (char i = 0; i < nWidgets; i++) {
    //sTopic[i]   = prefix + "/" + deviceID + "/" + descr[i] + "/" + String(id[i], DEC);
    sTopic[i]   = prefix + "/" + deviceID + "/" + descr[i];
    if (type_mqtt == 1) {
      sTopic[i] = deviceID + "/" + descr[i] + "/" + String(id[i], DEC);
    }
    //sTopic[i]   = prefix + "/" + String(id[i], DEC);
  }
  //  unsigned int *value_back = getSPIFFS_JSON_VALUE(nWidgets);


}
// send confirmation
String make_thing_config(char i) {
  String this_thing_config;
  //for (char i = 0; i < nWidgets; i++) {


  String ifsimple_btn;

  String widget_decode = "unknown";
  ifsimple_btn = "";
  switch (widget[i]) {
      break;
    case 1:
      widget_decode = "toggle";
      break;
    case 2:
      widget_decode = "simple-btn";
      ifsimple_btn = ",\"class3\":\"button button-balanced\"";
      ifsimple_btn += ",\"title\":\"" + descr[i] + "\"";
      //ifsimple_btn += ,\"label\":" + descr[i];
      break;
    case 3:
      widget_decode = "range";
      break;
    case 4:
      widget_decode = "small - badge";
      break;
    case 5:
      widget_decode = "chart";
      // ifsimple_btn = ", \"widgetConfig\":{\"type\": \"'line'\"}";
      break;
    case 6:
      widget_decode = "gauge";
      ifsimple_btn = ",\"class1\":\"text-center col-xs-6 no-padding-left no-padding-right\"";
      break;
    default:
      widget_decode = "unknown";

      break;
  }
  //    thing_config[i] = "{\"id\":\"" + String(id[i], DEC) + "\",\"page\":\"" + deviceID + "\",\"descr\":\"" + descr[i] + "\",\"widget\":\"" + widget_decode + "\",\"topic\":\"" + sTopic[i] + "\"" + ifsimple_btn + "}"; // GPIO switched On/Off by mobile widget toggle
  //  this_thing_config = "{\"id\":\"" + String(id[i], DEC) + "\",\"page\":\"" + deviceID + "\",\"descr\":\"" + descr[i] + "\",\"widget\":\"" + widget_decode + "\",\"topic\":\"" + sTopic[i] + "\"" + ifsimple_btn + "}"; // GPIO switched On/Off by mobile widget toggle
  this_thing_config = "{\"id\":\"" + String(id[i], DEC) + "\",\"page\":\"" + deviceID + "\",\"descr\":\"" + descr[i] + "\",\"widget\":\"" + widget_decode + "\",\"topic\":\"" + sTopic[i] + "\"" + ifsimple_btn + "}"; // GPIO switched On/Off by mobile widget toggle

  //}

  return this_thing_config;
}
void pubStatus(String t, String payload) {
  if (!client.connected())  return;

  if (type_mqtt == 0) {
    t += "/status";
  }

  if (client.publish(t.c_str(), payload.c_str())) {
    Serial.println("Publish new status for " + t + ", value: " + payload);
  } else {
    Serial.println("Publish new status for " + t + " FAIL!");
  }
}
void pubConfig() {
  bool success;

  /*
    char __prefix[prefix.length() + 1];
    prefix.toCharArray(__prefix, prefix.length() + 1);

    char __deviceID[deviceID.length() + 1];
    deviceID.toCharArray(__deviceID, prefix.length() + 1);
  */

  const char* __prefix = prefix.c_str();
  const char* __deviceID = deviceID.c_str();

  success = client.publish(__prefix, __deviceID);

  if (success) {
    if (type_mqtt == 0) {
      delay(500);
      String topic_str = prefix + "/" + deviceID + "/config";
      const char * __topic_str = topic_str.c_str();
      for (int i = 0; i < char(nWidgets); i = i + 1) {
        String msg_str = make_thing_config(i);
        /*
              char __topic_str[topic_str.length() + 1];
              topic_str.toCharArray(__topic_str, topic_str.length() + 1);

              char __msg_str[msg_str.length() + 1];
              msg_str.toCharArray(__msg_str, msg_str.length() + 1);
        */

        const char * __msg_str = msg_str.c_str();

        //success = client.publish(__topic_str, __msg_str);
        if (client.publish(__topic_str, __msg_str)) {
          Serial.println("Publish config: Success (topic:" + String(__topic_str) + " msg:" + String(msg_str) + ")");
        } else {
          Serial.println("Publish config FAIL! (topic:" + String(__topic_str) + " msg:" + String(msg_str) + ")");
        }
        delay(150);
      }
    }
  }

  if (success) {
    Serial.println("Publish config: Success");
  } else {
    Serial.println("Publish config: FAIL");
  }
  for (char i = 0; i < nWidgets; i = i + 1) {
    if ((pinmode[i] == 2) || (pinmode[i] == 1)) {//out, in
      pubStatus(sTopic[i], setStatus(stat[i]^defaultVal[i]));
    } else {
      pubStatus(sTopic[i], setStatus(stat[i]));
    }




    delay(150);
  }
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void callback(char* topic_char, byte * Byte, unsigned char length) {

  char convertedChar[length];
  char oneByte;
  unsigned char i1;
  for (unsigned char i = 0; i < length; i++) {
    oneByte = (char)Byte[i];
    if ((oneByte >= '0' && oneByte <= 'z') || (oneByte == ' ')) {
      convertedChar[i1] = oneByte;
      i1++;
    }
    else {
      Serial.println("!!break at:" + String(i1, DEC) + " position");
      break;
    }
  }
  convertedChar[i1] = '\0';
  //Serial.println("convertedChar:" + String(convertedChar));

  String topic = String(topic_char);
  String payload = String(convertedChar);
  int newValue = payload.toInt();
  //Serial.print("Get data from subscribed topic ");
  Serial.print(topic);
  Serial.print(" => ");
  Serial.println(payload);
  //pubStatus(deviceID + "/PLUS/" + String(thatCondition, DEC) + "/" + String(i, DEC), setStatus(bySignalPWM[thatCondition][i]));

  String deviceID_topic = getValue(topic, '/', 0);
  String option_topic = getValue(topic, '/', 1);
  String thatCondition = getValue(topic, '/', 2);
  String thatConditionNumber = getValue(topic, '/', 3);
  Serial.println(deviceID_topic + " " + option_topic + " " + thatCondition + " " + thatConditionNumber);
  if (option_topic == "PLUS") {
    Serial.println("Set PLUS cond:" + thatCondition + " n:" + thatConditionNumber);
    bySignalPWM[thatCondition.toInt()][thatConditionNumber.toInt()] = newValue;
    saveConditiontoJson(thatCondition.toInt());
  }
  for (char i = 0; i < nWidgets; i++) {
    //Serial.println("i:" + String(i, DEC) + " topic:" + topic + " " + sTopic[i] + "/control");
    String compare;

    if (type_mqtt == 0) {
      compare = sTopic[i] + "/control";
    } else if (type_mqtt == 1) {//MajorDomo
      compare = sTopic[i] + "/status";
    }


    if (topic == compare) {
      stat[i] = (newValue);
      if ((pinmode[i] == 2) || (pinmode[i] == 1)) {//out, in
        stat[i] = newValue ^ defaultVal[i];
        send_IR(i);
        digitalWrite(pin[i], stat[i]);
      }
      else if (pinmode[i] == 3) {//pwm
        analogWrite(pin[i], newValue);
      }
      else if (pinmode[i] == 5) {//low_pwm
        low_pwm[i] = newValue;
      }
      else if (pinmode[i] == 4) { //adc
      }
      pubStatus(sTopic[i], setStatus(stat[i]));
      break;
    }
  }


  if (topic == prefix) {
    if ((char)Byte[0] == 'H') {//???????????? HELLO
      if (type_mqtt == 0) {
        pubConfig();
      }
    }
  }
}
void  ICACHE_RAM_ATTR do_on_delay()
{
  if (zero_cross == true) {
    for (char i = 0; i < nWidgets; i++) {
      if (pinmode[i] == 10) {
        if (i_micros_dimming >= stat[i]) {
          digitalWrite(pin[i], HIGH); // turn on light
          i_micros_dimming = 0; // reset time step counter
          zero_cross = false; //reset zero cross detection
          delayMicroseconds(10);         // triac On propogation delay
          digitalWrite(pin[i], LOW); // turn on light
        } else {
          i_micros_dimming++; // increment time step counter
          digitalWrite(pin[i], LOW); // turn on light
        }
        break;
      }
    }
  }
}
void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  zero_cross = true;
  i_micros_dimming = 0;
  //InitInterrupt(do_something);
  for (char i = 0; i < nWidgets; i++) {
    if (pinmode[i] == 10) {
      digitalWrite(pin[i], LOW); // turn on light
      break;
    }
  }
  InitInterrupt(do_on_delay, freqStep);
}

void InitInterrupt(timercallback handler, int Step )
{
  timer1_disable();
  timer1_isr_init();
  timer1_attachInterrupt(handler);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(Step);//max8388607//75*5
}


void Setup_pinmode() {
  for (char i = 0; i < char(nWidgets); i++) {
    if (((pin[i] >= 6) && (pin[i] <= 9)) || (pin[i] == 11) || (pin[i] == 255)) {
      break;
    }
    if (pinmode[i] == 1) {//in
      pinMode(pin[i], INPUT);
      stat[i] = (digitalRead(pin[i]));
      Serial.println("set input:" + String(pin[i], DEC) + "i:" + i);
    }
    if ((pinmode[i] == 2)) { //out
      pinMode(pin[i], OUTPUT);
      digitalWrite(pin[i], stat[i]);
      //stat[i] = (defaultVal[i]);
      Serial.println("set output:" + String(pin[i], DEC) + "i:" + i + "stat:" + stat[i]);
    }
    if ((pinmode[i] == 3) || (pinmode[i] == 7)) { //pwm,MQ7
      pinMode(pin[i], OUTPUT);
      analogWrite(pin[i], stat[i]); // PWM
      //analogWrite(pin[i], 286);//1.4V
      Serial.println("set pwm:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String(stat[i], DEC));
    }
    if (pinmode[i] == 5) {//low_pwm
      pinMode(pin[i], OUTPUT);
      low_pwm[i] = stat[i];
      digitalWrite(pin[i], 1);//?????????? - ??????????????????
      Serial.println("set low_pwm:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String(stat[i], DEC));
    }
    if (pinmode[i] == 4) {//adc
      stat[i] = (analogRead(pin[i]) / analogDivider) + analogSubtracter; //adc pin:A0
      Serial.println("read adc:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String( stat[i], DEC));
    }
    if (pinmode[i] == 6) {//dht temp
      dht.setup(pin[i]); // data pin
      Serial.println("DHT:" + String(pin[i], DEC) );
    }
    if (pinmode[i] == 8) {//dht hum
      dht.setup(pin[i]); // data pin
    }
    if (pinmode[i] == 9) {//220interrupt
      //attachInterrupt(pin[i], zero_crosss_int, RISING);//When arduino Pin 2 is FALLING from HIGH to LOW, run light procedure!
      //InitInterrupt(do_on_delay, freqStep);
    }
    if (pinmode[i] == 10) { //pwm 220
      //pinMode(pin[i], OUTPUT);
    }
  }

}

void setup_IOTManager() {


  const char *__mqttServerName = mqttServerName.c_str();
  //const char *_payload = payload.c_str();

  Serial.println("MQTT Server:" + String(__mqttServerName) + " port:" + mqttport);
  client.setServer(__mqttServerName, mqttport);
  client.setCallback(callback);
  if (loadWidgets()) {
    Serial.println("widgets Loaded");
  }
  else {
    initVar();
  }

}



void loop_IOTMAnager() {

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.print("Connecting to MQTT server: ");
      Serial.print(mqttServerName);
      Serial.print(" port ");
      Serial.println(mqttport);
      bool success = false;
      //if (internet) {
      const char *__deviceID = deviceID.c_str();
      if (mqttuser.length() > 0) {

        const char *__mqttuser = mqttuser.c_str();
        const char *__mqttpass = mqttpass.c_str();

        Serial.println("MQTT device id:" + String(__deviceID) + ", user:" + String(__mqttuser) + ",password:" + String(__mqttpass));

        //success = client.connect( MQTT::Connect( deviceID ).set_auth(mqttuser, mqttpass) );
        success = client.connect(__deviceID , __mqttuser, __mqttpass );
      } else {
        success = client.connect(__deviceID);
      }
      // }
      if (success) {
        server.send(200, "text/json", "{client:connected}");
        Serial.println("Connect to MQTT server: Success");
        pubConfig();
        subscribe_loop = 0;
        const char *__prefix = prefix.c_str();
        if (__prefix.length() > 0) {
          if (!client.subscribe(__prefix)) {
            Serial.println("FAIL! subscribe" + String(__prefix));
            trying_attempt_mqtt = 5;
          };                 // for receiving HELLO messages
        }
        //client.subscribe(prefix + "/ids");        // for receiving IDS  messages
        for (int i = 0; i < char(nWidgets); i++) {
          /*

            if (i < nWidgets) {
            String topic_subscr;
            if (type_mqtt == 0) {//Iotmanager
              topic_subscr = sTopic[i] + "/control";
            } else if (type_mqtt == 1) {//MajorDomo
              topic_subscr = sTopic[i] + "/status";
            }
            const char *__topic_subscr = topic_subscr.c_str();

            if (!client.subscribe(__topic_subscr)) {
              Serial.println("Client subscribe FAIL CYCLE!:" + String(__topic_subscr));
              subscribe_loop = i;
              break;
            }          else {

              Serial.println("Client subscribe SUCCSESS CYCLE!:" + String(__topic_subscr));
            };   // for receiving GPIO messages
            //delay(10000);
            }
          */
        }
      } else {
        success = false;
        //long wait_for_sec = ((no_internet_timer + 30000) - millis()) / 1000;
        Serial.print("Connect to MQTT server: FAIL");
        try_MQTT_access = false;
        //////////////////////////
        trying_attempt_mqtt++;
        if (trying_attempt_mqtt > 5) { //?????????? ?????? ??????????????????
          if (internet_pin != 255) {
            internet_pin_inv = digitalRead(internet_pin); //???????????????????? ?? Wifi ????????, ???????????? ???????????? ??????????????
            digitalWrite(internet_pin, internet_pin_inv ^ 1);//?????????????????? ????????????
            trying_attempt_mqtt = 0;
            internet_cycle = 1;
          }
        }
        //////////////////////////
      }

    }

    if (client.connected()) {
      newtime = millis();
      if ((newtime - oldtime > subscr_sec)) { // 20 sec
        //////////////subscribe///////////////////////////
        if (subscribe_loop < nWidgets) {
          //if (subscribe_loop > nWidgets)return;
          String topic_subscr;
          if (type_mqtt == 0) {//Iotmanager
            topic_subscr = sTopic[subscribe_loop] + "/control";
          }
          else if (type_mqtt == 1) {//MajorDomo
            topic_subscr = sTopic[subscribe_loop] + "/status";
          }
          const char *__topic_subscr = topic_subscr.c_str();
          if (!client.subscribe(__topic_subscr)) {
            Serial.println("Client subscribe FAIL!:" + String(__topic_subscr));
          }
          else {
            Serial.println("Client subscribe SUCCSESS!:" + String(__topic_subscr));
            subscribe_loop++;
          };

          Serial.println(subscr_sec);
          subscr_sec =  (((mqttspacing * 1000L) / nWidgets)) * subscribe_loop;
        }
        /////////////////////////////////////////
      }
      if ((newtime - oldtime > mqttspacing * 1000L) && (!ESP_busy)) { // 20 sec
        float x = 0.0f;
        for (int i = 0; i < nWidgets; i++) {
          if ((pinmode[i] == 2) || (pinmode[i] == 1)) {//out, in
            x = digitalRead(pin[i]);
          }
          else if (pinmode[i] == 4) {//adc
            x = (analogRead(pin[i]) * 1.0F / analogDivider * 1.0F) + analogSubtracter;
          }
          else if (pinmode[i] == 6) {
            delay(dht.getMinimumSamplingPeriod());
            x = dht.getTemperature();

          }
          else if (pinmode[i] == 8) {
            delay(dht.getMinimumSamplingPeriod());
            x = dht.getHumidity();
          }
          if (isnan(x)) {
            x =  stat[i] = 0;
          }
          stat[i] = (int)x;
          pubStatus(sTopic[i],  setStatus(String(x, 2)));
        }
        oldtime = newtime;
        subscr_sec = 0;
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

