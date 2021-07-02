#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//IPAddress ip(192, 168, 1, 170);
IPAddress server_mqtt(192, 168, 1, 102);

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);
String ip = "";
EthernetClient ethClient;
PubSubClient PubClient(server_mqtt, 1883, callback, ethClient);

void setup()
{
  for (char i = 0; i < 14; i++) {
    pinMode(i, OUTPUT);
  }
  setup_server();
  Serial.begin(115200);
  //Serial.println("start..");
  Ethernet.begin(mac);
  Serial.println("Ethernet connected");
  ip = String (Ethernet.localIP()[0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[3]);
  Serial.println(ip);
  mqtt_setup();

}
void loop()
{
  loop_server();
  loop_mqtt();
  PubClient.loop();
}
