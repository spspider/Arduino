#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[700];
static uint32_t timer;
char website[] PROGMEM = "www.devicehub.net";


int state = 0;
// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.println((const char*) Ethernet::buffer + off + len - 1);
  state = *(const char*)(Ethernet::buffer + off + len - 1);
  Serial.println(state);
  if(state == 49){
    //actuator ON
    Serial.println("actuator ON");
    digitalWrite(3, HIGH);
  }else if(state == 48){
    //actuator OFF
    Serial.println("actuator OFF");
    digitalWrite(3, LOW);
  }
}


void setup()
{
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0)
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    Serial.println("line 1");
  ether.printIp("SRV: ", ether.hisip);
  Serial.println("turning on the led");
  digitalWrite(9, HIGH);
}

void loop()
{
  ether.packetLoop(ether.packetReceive());
  if (millis() > timer) {
    char queryString[256] = {0};
    sprintf(queryString, "?apiKey=0aabb23b-9d0e-48e9-88f8-f3a146471da2");

    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    ether.browseUrl(PSTR("/io/296/actuator/134/"), queryString, website, my_callback);
  }
}
