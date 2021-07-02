unsigned long buffer_mqtt_loop;
char* mqtt_PubClient = "mqtt_client";
void mqtt_setup() {

  char mqtt_user[20] = "mqtt_user";
  char mqtt_pass[20] = "mqtt_pass";
  bool auth = false;
  if (auth) {
    if (PubClient.connect(mqtt_PubClient, mqtt_user, mqtt_pass)) {}
  }
  else {
    Serial.println("connecting to " + Ethernet.localIP());
    if (PubClient.connect(mqtt_PubClient)) {
      Serial.println("connected");
      PubClient.publish("outTopic", "hello world");
      for (char i = 0; i < 4; i++) {
        String subscr_str = String(mqtt_PubClient) + "/" + "pin/" + String(i, DEC);
        if (PubClient.subscribe(subscr_str.c_str())) {
          Serial.println("SUBSCRIBE: " + subscr_str);
        }
      }
    }
    else {
      Serial.println("connection problem");
    }
  }
}
void sendNewStatus() {
  for (char i = 0; i < 5; i++) {
    if (PubClient.connected()) {
      if (i < 14) {
        String publish_str = String(mqtt_PubClient) + "/" + "pin/" + String(i, DEC);
        if (PubClient.publish(publish_str.c_str(), String(analogRead(i)).c_str())) {
          Serial.println(publish_str + String(digitalRead(i)));
        }
      }
      else {
        String publish_str = String(mqtt_PubClient) + "/" + "ADC/" + String(i, DEC);
        PubClient.publish(publish_str.c_str(), String(analogRead(i)).c_str());
      }
    }
  }
}
void loop_mqtt() {
  if (millis() > buffer_mqtt_loop +  60L * 1000L ) { //каждые 1 мин
    sendNewStatus();
    buffer_mqtt_loop = millis();
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
void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length);

  // Copy the payload to the new buffer

  memcpy(p, payload, length);

  char convertedChar[length];
  char oneByte;
  unsigned char i1;
  for (unsigned char i = 0; i < length; i++) {
    oneByte = (char)payload[i];
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

  Serial.print("topic:");
  Serial.println(topic);
  Serial.print("payload:");
  Serial.println(convertedChar);
  int value = String(convertedChar).toInt();
  //String(mqtt_PubClient) + "/" + "pin/" + String(i, DEC);
  String deviceID = getValue(topic, '/', 0);
  String type_control = getValue(topic, '/', 1);
  String pin_string = getValue(topic, '/', 2);
  char pin = pin_string != "" ? pin_string.toInt() : 255;
  if (deviceID == String (mqtt_PubClient)) {
    if (pin != 255) {
      if (value <= 1) {//digital
        digitalWrite(pin, value);
        Serial.println("Set pin:" + pin_string + " value:" + String(value));
      } else if
      (value > 1 ) { //analog
        analogWrite(pin, value);
        Serial.println("Set pin analogWrite:" + pin_string + " value:" + String(value));
      }
    }
  }
  //PubClient.publish("outTopic", p, length);
  // Free the memory
  free(p);
}
