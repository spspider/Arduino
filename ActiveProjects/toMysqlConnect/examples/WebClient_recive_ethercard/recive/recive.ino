#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "arduinowebserver.hol.es";    // name address for Google (using DNS)
IPAddress ip(192, 168, 1, 177);

EthernetClient client;
TextFinder finder( client );
const int NUMBER_OF_FIELDS = 8; // сколько полей искать
long values[NUMBER_OF_FIELDS];//массив с найденными цифрами
void setup() {
  
Serial.begin(9600);//открыть пследовательный порт

  if (Ethernet.begin(mac) == 0) {//старт интернета
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);//не получилось, пробуем через IP
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /index.php HTTP/1.1");
    client.println("Host: www.arduinowebserver.hol.es");
    client.println("Connection: close");
    client.println();
    //закончили соединение
    ////////////////////////////////////////////////////
  //while(finder.findUntil("<td>", "</td>"))
      //{
      //  int i;
      // values[i++] = finder.getValue(); 
      // Serial.print(values[i]);
      // Serial.println(" data1");
     // }   
     for (int i=0; i <= 3; i++){
//     finder.find("<td>");  // что искать
     finder.findUntil("<td>","</td>");  // до чего
     values[i] = finder.getValue();//переводим все данные в массив
    Serial.print("i:");
    Serial.print(i);Serial.print(" ");
    Serial.print(values[i]);Serial.print(":");
    Serial.println(" data1\n");
    }
 
 }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{

 
  if (client.available()) {
      
  }
 

  // if the server's disconnected, stop the client:
  if (!client.connected()) {


    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}

