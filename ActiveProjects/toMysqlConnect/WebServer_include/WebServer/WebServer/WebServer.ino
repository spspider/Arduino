#include <SPI.h>
//#include <Timer.h>
#include <EEPROM.h> 
#include <avr/wdt.h>
#include <Ethernet.h>
#include <TextFinder.h>
//#include <server_file.ino>
//#include <client_file.ino>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server_my[] = "arduinowebserver.hol.es";    // name address for Google (using DNS)
IPAddress ip(192, 168, 1, 177);
int pin[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13};
//int exclude_pin[10];
//int exclude_pin[10];
EthernetServer server(80);
EthernetClient client_my;
//Client client_my(server_my,80);
//TextFinder finder_numfields( client_my,5 );
TextFinder finder_fields( client_my,5 );
//const int NUMBER_OF_FIELDS = 8; // сколько полей искать
unsigned int values[5][15];//массив с найденными цифрами
unsigned char analog_values[5];
//int ststus
unsigned char now_server=0;



void setup() {
//wdt_enable(WDTO_8S);
//IRremote_setup();



Serial.begin(9600);//открыть пследовательный порт
 if (now_server==1){server_setup(); }
 else{client_setup(); }}
 
void loop()
{
  //IRremote_loop();
  wdt_reset();
  //pulse_loop(2,900,1000);
  //pulse_loop(4,150,600);

  for (int i=0;i<14;i++){
    //if (values[1][i]==2){
    //pulse_loop(i,values[2][i],values[3][i]);
   // }
   if (values[1][i]==3){
    analogWrite(i, values[2][i]);
   }
    if (values[1][i]==4){
    //analogRead(i); 
   }
  }
  //pulse_loop(2,1,5);//exclude_pin[4]=1;
  if (now_server!=1)//если сейчас работает сервер
    {client_loop();}
  else
    {server_loop();}//если подключен как клиент
}

