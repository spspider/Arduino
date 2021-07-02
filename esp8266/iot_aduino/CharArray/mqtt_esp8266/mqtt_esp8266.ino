/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <string.h>
// Update these with values suitable for your network.

const char* ssid = "Dom";
const char* password = "paukov99";
const char* mqtt_server = "m20.cloudmqtt.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 16238);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  int pay_int = int(payload);
  Serial.println(pay_int);
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", "spspider", "5506487")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String topic_str_status = "/status";
      const char * topic_str_status_const = topic_str_status.c_str();
      client.publish(topic_str_status_const, "hello world");
      // ... and resubscribe

      if (!client.subscribe("inTopic1")) {
        Serial.println("Fail Subscribe inTopic1");
      }
      if (!client.subscribe("inTopic2")) {
        Serial.println("Fail Subscribe inTopic2");
      }
      if (!client.subscribe("inTopic3")) {
        Serial.println("Fail Subscribe inTopic3");
      }
      if (!client.subscribe("inTopic4")) {
        Serial.println("Fail Subscribe inTopic4");
      }
      if (!client.subscribe("inTopic5")) {
        Serial.println("Fail Subscribe inTopic5");
      }
      if (!client.subscribe("inTopic6")) {
        Serial.println("Fail Subscribe inTopic6");
      }
      if (!client.subscribe("inTopic7")) {
        Serial.println("Fail Subscribe inTopic7");
      }
      if (!client.subscribe("inTopic8")) {
        Serial.println("Fail Subscribe inTopic8");
      }
      if (!client.subscribe("inTopic9")) {
        Serial.println("Fail Subscribe inTopic9");
      }
      if (!client.subscribe("inTopic10")) {
        Serial.println("Fail Subscribe inTopic10");
      }
      if (!client.subscribe("inTopic11")) {
        Serial.println("Fail Subscribe inTopic11");
      }
      if (!client.subscribe("inTopic12")) {
        Serial.println("Fail Subscribe inTopic12");
      }
      if (!client.subscribe("inTopic13")) {
        Serial.println("Fail Subscribe inTopic13");
      }
      if (!client.subscribe("inTopic14")) {
        Serial.println("Fail Subscribe inTopic14");
      }
      /*
        for (int i = 0; i < 6; i++) {
        String Topic = "inTopic"+i;
        char _topic[Topic.length()+1];
        Topic.toCharArray(_topic, (Topic.length()+1));
        if (!client.subscribe(_topic)) {
          Serial.println("Fail Subscribe inTopic"+i);
        }
        }
      */
      //if(waitmillisec_for_subscr-millis()>)
      // for (int i = 0; i < 10; i++) {
      long delay_time = 10000L;
      long     pass_time = 0L;

      /*
            if (pass_time > delay_time) {
              String Topic = "topic" + i;
              char _topic[Topic.length() + 1];
              Topic.toCharArray(_topic, Topic.length() + 1);
              //const char *_topic = Topic.c_str();
              if (client.subscribe(_topic)) {

              }
              else {
                Serial.println("Fail Subscribe" + String(_topic));
              };
              //delay(1000);
              //}
              i++;

            }
      */
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
