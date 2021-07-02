#include <SPI.h>
#include <Ethernet.h>
#include <Utility\Socket.h>

// Make up a mac Address & an IP address. Both should be globally unique or
// at least unique on the local network. 
//static byte g_abyMyMacAddress[] = {0x00,0x1A,0x4B,0x38,0x0C,0x5C};
//static IPAddress g_MyIPAddress(192,168,15,44);

// The machine to wake up. WOL should already be configured for the target machine. 
// The free windows program "Wake On LAN Ex 2" by Joseph Cox can be useful for testing the remote
// machine is properly configured. Download it here: http://software.bootblock.co.uk/?id=wakeonlanex2
static byte g_TargetMacAddress[] = {0x54,0x04,0xA6,0xF2,0x2F,0x5A};
//1C-3E-84-4C-A3-1D
//54-04-A6-F2-2F-5A
 
void loop_wake() 
{
 
  // When 'w' is received, send a magic packet to wake the remote machine. 
  if(Serial.available() > 0) 
    if(Serial.read() == 'w') 
    {
      SendWOLMagicPacket(g_TargetMacAddress);
      Serial.println("Magic packet sent");
    }
}

void SendWOLMagicPacket(byte * pMacAddress)
{
  // The magic packet data sent to wake the remote machine. Target machine's
  // MAC address will be composited in here.
  const int nMagicPacketLength = 102;
  byte abyMagicPacket[nMagicPacketLength] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  byte abyTargetIPAddress[] = { 255, 255, 255, 255 }; // don't seem to need a real ip address.
  const int nWOLPort = 7;
  const int nLocalPort = 8888; // to "listen" on (only needed to initialize udp)

  
  // Compose magic packet to wake remote machine. 
  for (int ix=6; ix<102; ix++)
    abyMagicPacket[ix]=pMacAddress[ix%6];
  
  if (UDP_RawSendto(abyMagicPacket, nMagicPacketLength, nLocalPort, 
  abyTargetIPAddress, nWOLPort) != nMagicPacketLength)
    Serial.println("Error sending WOL packet");
}

int UDP_RawSendto(byte* pDataPacket, int nPacketLength, int nLocalPort, byte* pRemoteIP, int nRemotePort)
{
  int nResult;
  int nSocketId; // Socket ID for Wiz5100

  // Find a free socket id.
  nSocketId = MAX_SOCK_NUM;
  for (int i = 0; i < MAX_SOCK_NUM; i++) 
  {
    uint8_t s = W5100.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) 
	  {
      nSocketId = i;
      break;
    }
  }

  if (nSocketId == MAX_SOCK_NUM)
    return 0; // couldn't find one. 

  if (socket(nSocketId, SnMR::UDP, nLocalPort, 0))
  {
    nResult = sendto(nSocketId,(unsigned char*)pDataPacket,nPacketLength,(unsigned char*)pRemoteIP,nRemotePort);
    close(nSocketId);
  } else
    nResult = 0;

  return nResult;
}

