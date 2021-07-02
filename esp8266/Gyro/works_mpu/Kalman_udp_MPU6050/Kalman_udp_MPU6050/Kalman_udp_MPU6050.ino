
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <AsciiMassagePacker.h>
const char* ssid = "Home";
const char* password = "sp5506487";

WiFiUDP Udp;
unsigned int localUdpPort = 10000;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
AsciiMassagePacker outbound;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  setup_mpu();
}
bool sendPacket(const IPAddress& address, const uint8_t* buf, uint8_t bufSize) {
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(buf, bufSize);
  return (Udp.endPacket() == 1);
}

void loop()
{

  /*
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacekt);
    Udp.endPacket();
  }
  */
    loop_mpu();
}
