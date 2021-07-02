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
//PubSubClient client(wclient); // for cloud broker - by hostname


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

char nWidgets = 0;
const char nWidgetsArray = 0;
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
unsigned char   pinmode[nWidgetsArray];//inputPinmode[0] = "no pin";inputPinmode[1] = "in";inputPinmode[2] = "out";inputPinmode[3] = "pwm";inputPinmode[4] = "adc";inputPinmode[5] = "low_pwm";inputPinmode[6] = "IR";inputPinmode[7] = "датчик газа MQ7";

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
  /*
  if (!client.connected())  return;

  if (type_mqtt == 0) {
    t += "/status";
  }

  if (client.publish(t.c_str(), payload.c_str())) {
    Serial.println("Publish new status for " + t + ", value: " + payload);
  } else {
    Serial.println("Publish new status for " + t + " FAIL!");
  }
  */
}
void pubConfig() {
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

}


void Setup_pinmode() {
  for (char i = 0; i < char(nWidgets); i++) {
    if (((pin[i] >= 6) && (pin[i] <= 9)) || (pin[i] == 11) || (pin[i] == 255)) {
      break;
    }
    if (pinmode[i] == 1) {//in
      defaultVal[i] == 0 ? pinMode(pin[i], INPUT_PULLUP) : pinMode(pin[i], INPUT);
      stat[i] = (digitalRead(pin[i] ^ defaultVal[i]));
      //Serial.println("set input:" + String(pin[i], DEC) + "i:" + i);
    }
    if ((pinmode[i] == 2)) { //out
      pinMode(pin[i], OUTPUT);
      //stat[i] =  (defaultVal[i]);
      digitalWrite(pin[i], stat[i]^defaultVal[i]);//^defaultVal[i]
      //stat[i] = (defaultVal[i]);
      //Serial.println("set output:" + String(pin[i], DEC) + "i:" + i + "stat:" + stat[i] + "def:" + String(defaultVal[i], DEC));
    }
    if ((pinmode[i] == 3) || (pinmode[i] == 7)) { //pwm,MQ7
      pinMode(pin[i], OUTPUT);
      analogWrite(pin[i], stat[i]); // PWM
      //analogWrite(pin[i], 286);//1.4V
      //Serial.println("set pwm:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String(stat[i], DEC));
    }
    if (pinmode[i] == 5) {//low_pwm
      pinMode(pin[i], OUTPUT);
      low_pwm[i] = stat[i];
      digitalWrite(pin[i], 1);//далее - выключаем
      //Serial.println("set low_pwm:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String(stat[i], DEC));
    }
    if (pinmode[i] == 4) {//adc
      stat[i] = (analogRead(17) * 1.0F / analogDivider) + analogSubtracter; //adc pin:A0
      //Serial.println("read adc:" + String(pin[i], DEC) + "i:" + String(i, DEC) + "stat:" + String( stat[i], DEC));
    }
    if (pinmode[i] == 9) {//220interrupt
      //attachInterrupt(pin[i], zero_crosss_int, RISING);//When arduino Pin 2 is FALLING from HIGH to LOW, run light procedure!
      //InitInterrupt(do_on_delay, freqStep);
    }

  }

}

void setup_IOTManager() {


    initVar();
  

}



void loop_IOTMAnager() {

}
float get_new_pin_value(uint8_t i) {
  float that_stat = 0.0f;
  that_stat = stat[i];
  switch (pinmode[i]) {
    case 1://in
      that_stat = digitalRead(pin[i])^defaultVal[i];
      stat[i] = that_stat;
      break;
    case 2://out
      // that_stat = digitalRead(pin[i])^defaultVal[i];
      //stat[i] = that_stat;
      break;
    case 3:
      break;
    case 4://adc
      that_stat = (analogRead(pin[i]) * 1.0F / analogDivider * 1.0F ) + analogSubtracter; //adc pin:A0
      stat[i] = (int)that_stat;
      break;
      break;
    case 9://remote
      that_stat = getHttp(descr[i]).toFloat();
      break;

  }
  //that_stat = (isnan(that_stat) || isnanf (that_stat)) ? 0 : that_stat;
  if ((isnan(that_stat)) || ( isinf (that_stat))) {
    that_stat =  stat[i];//0
  }

  return that_stat;
}

