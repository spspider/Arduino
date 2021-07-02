#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <WakeOnLan.h>

WiFiUDP UDP;
WakeOnLan WOL(UDP);


void wakeMyPC(const char* MACAddress) {
  //MACAddress = "54:04:A6:F2:2F:5A";
  for (uint8_t i = 0; i < 3; i++) {
    WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
    delay(100);
  }
  //return 1;
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}

void wakeOfficePC() {
  const char *MACAddress = "01:23:45:67:89:AB";
  const char *secureOn = "FE:DC:BA:98:76:54";

  WOL.sendSecureMagicPacket(MACAddress, secureOn); // Send Wake On Lan packet with the above MAC address and SecureOn feature. Default to port 9.
  // WOL.sendSecureMagicPacket(MACAddress, secureOn, 7); // Change the port number
}

void setup_WOL()
{
  WOL.setRepeat(3, 100); // Optional, repeat the packet three times with 100ms between. WARNING delay() is used between send packet function.

  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); // Optional  => To calculate the broadcast address, otherwise 255.255.255.255 is used (which is denied in some networks).

  //  wakeMyPC();
  //wakeOfficePC();
}

