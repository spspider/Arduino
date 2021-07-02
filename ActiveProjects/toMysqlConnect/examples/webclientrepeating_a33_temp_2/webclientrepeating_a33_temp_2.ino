/*
  Repeating Web client
 
 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.
 
 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 19 Apr 2012
 by Tom Igoe
 
 http://arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example  
//

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(10,0,5,11);

// fill in your Domain Name Server address here:
IPAddress myDns(192,168,90,201);

float celsius, fahrenheit;
OneWire  ds(8);  // on pin 8 (a 4.7K resistor is necessary)

// initialize the library instance:
EthernetClient client;

char server[] = "a33.p.ht";

   //prepear to start compare line
boolean PinOut1 = FALSE; //is set pin to ON?
boolean PinOut0 = FALSE; //is set pin to OFF
int index1 = 0;      //index to find command ON
int index0 = 0;      //index to find command OFF
char BufPinOut0[] = "out1=0";    //command string OFF
char BufPinOut1[] = "out1=1";    //command string ON
int Len_BufPinOut0 = 6;          //long
int Len_BufPinOut1 = 6;

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 15*1000;  // delay between updates, in milliseconds

void setup() {
  // start serial port:
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
  digitalWrite(7, 1); 
  digitalWrite(9, 0); 
   //prepear to start compare line
   PinOut1 = FALSE; //is set pin to ON?
   PinOut0 = FALSE; //is set pin to OFF
   index1 = 0;      //index to find command ON
   index0 = 0;      //index to find command OFF

  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    
    if (!PinOut1)
        {
          if (c == BufPinOut1[index1]) { index1 ++; } 
          else  { index1 = 0; } 
          if (index1 == Len_BufPinOut1) 
             {PinOut1 = TRUE;
              Serial.println("Find command to ON.");}
        }  

    if (!PinOut0)
        {
          if (c == BufPinOut0[index0]) { index0 ++; } 
          else  { index0 = 0; } 
          if (index0 == Len_BufPinOut0) 
              {PinOut0 = TRUE;
              Serial.println("Find command to OFF.");}
        }  
    //set output pin
    if (PinOut0) { digitalWrite(9, 0); }
    if (PinOut1) { digitalWrite(9, 1); }

  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    Measure_Temperature();
    httpRequest();
    
   //prepear to start compare line
   PinOut1 = FALSE; //is set pin to ON?
   PinOut0 = FALSE; //is set pin to OFF
   index1 = 0;      //index to find command ON
   index0 = 0;      //index to find command OFF
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
//  Serial.println("..no function");
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    int val7 = digitalRead(7);
    int val9 = digitalRead(9);
    // send the HTTP PUT request:
    client.print("GET /avr.php?in1=");
    client.print(val7);  //pin 7 input
    client.print("&out1=");
    client.print(val9);  //pin 9 output
    client.print("&temp1=");
    client.print(celsius);
    client.println(" HTTP/1.1");
    
    client.println("Host: a33.p.ht");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

void Measure_Temperature() {
//for 1-wire  (ds18b20)
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}


