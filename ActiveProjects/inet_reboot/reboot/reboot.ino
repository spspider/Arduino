/*
 * Simpleclient_myTextFinder.pde
 *
 */

#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018 
#endif


#include <Ethernet.h>
#include <TextFinder.h>
#include <avr/wdt.h>
#include "Timer.h"
#include <EEPROM.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1,177 };
byte gateway[] ={ 192, 168, 1, 1 };
byte subnet[] ={ 255, 255, 255, 0 };
byte IsRebootAfterLoad=0;
byte successfull_connected=0;
char server[] = "www.google.com";
char server_checkip[] = "checkip.dyndns.org";
unsigned int waiting_connection=0;
char externalIP[17];
char c = ' ';
int i=0;
long starttime = 0,off_time=0;
byte timer_off=0;
Timer t;
EthernetClient client_my;
TextFinder finder(client_my);
//wdt_enable(WDTO_8S);
long waiting_connection_2 =0;
void doCheckConnection()
{
  Serial.println("start check if internet is on");
  IsRebootAfterLoad=1;

}





void setup()
{
  successfull_connected=0;
  IsRebootAfterLoad=0;
  Ethernet.begin(mac, ip); 
  Serial.begin(9600);
  Serial.println("start...");  
  delay(1000);
  pinMode(9, OUTPUT);
  wdt_enable(WDTO_8S);
  digitalWrite(9, HIGH);
  
  
  int waiting_connection = t.after(1000L*60L*2L, doCheckConnection); 
  int update_internet = t.every(1L*60L*1000L,  connect_internet);
//  int take_ip_new = t.every(10000,  take_ip); 
 // waiting_connection_2 = millis();  
}

void loop()
{
 //   { if ((millis() - waiting_connection_2) == 1000L*20L)   Serial.println("20 sec gone");  }
  email_loop();
  t.update();
  wdt_reset(); 


 
}


void connect_internet(){
    if (client_my.connected()) {
    client_my.stop();
    client_my.flush();  
    }
    if (client_my.connect(server,80)) {
    Serial.print("search... ");
    client_my.println("GET /");
    client_my.println();
    //delay(1000);
  } else {
    Serial.print("search failed");
  }
/*  
starttime = millis();
  while (!client_my.available()) 
    { if ((millis() - starttime) > 100000)
       {
         Serial.println("Timeout");
         setup();
         client_my.stop();
         client_my.flush(); 
         break; break;
         
       }
    }
 */    
  if (client_my.connected()) {
  //check_ip();
  wdt_reset();
  //IsRebootAfterLoad=0;
  Serial.print("internet is on");
  Serial.print("\t");
  Serial.print("is reboot:");
  Serial.println(IsRebootAfterLoad, DEC);
  if (successfull_connected==0){
    successfull_connected=1;
    IsRebootAfterLoad=1;
    client_my.stop();
    client_my.flush();
    //timer_off=EEPROM.read(1);
    //off_time=EEPROM.read(0);
    if (timer_off==1){
      off_time=EEPROM.read(0)+(millis()-off_time)/1000L;//sec
      timer_off=0;}
      if (off_time!=0){
      sendEmail();
      Serial.println("Connected!Email Sended");
      Serial.println(off_time);
      
      //break;
      EEPROM.write(0,0);
      }
    }
  
 
  
 }
  else {
    //Serial.print();
    //successfull_connected=0;
    Serial.println("internet is off");
    if (timer_off==0){
       timer_off=1;
       off_time=off_time+millis();
       EEPROM.write(0, off_time);
       //EEPROM.write(1, timer_off);
       }
    if(IsRebootAfterLoad==1){    
    digitalWrite(9, LOW);
    //delay(10000);//loop for reset
    //MCUSR = 0;
    Serial.println("reset");
    IsRebootAfterLoad=0;
    successfull_connected=0;
    setup();//if loop failed.
}
  Serial.print("is reboot:");
  Serial.println(IsRebootAfterLoad, DEC);

  } 

  client_my.stop();
  client_my.flush();  
  }


