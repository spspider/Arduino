#include <SPI.h>
#include <EEPROM.h> 
#include <avr/wdt.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <server_file.ino>
#include <client_file.ino>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server_my[] = "arduinowebserver.hol.es";    // name address for Google (using DNS)
IPAddress ip(192, 168, 1, 177);
int pin[]={0,1,2,3,4,5,6,7,8,9,10};

EthernetServer server(80);
EthernetClient client_my;
TextFinder finder_numfields( client_my,1 );
TextFinder finder_fields( client_my,1 );
const int NUMBER_OF_FIELDS = 8; // сколько полей искать
int values[4][11];//массив с найденными цифрами
//int ststus
int now_server=0;



void setup() {
wdt_enable(WDTO_8S);




Serial.begin(9600);//открыть пследовательный порт
 if (now_server==1){server_setup(); }
 else{client_setup(); }}
 
void loop()
{
  wdt_reset();
  if (now_server!=1)//если сейчас работает сервер
    {client_loop();}
  else
    {server_loop();}//если подключен как клиент
}

