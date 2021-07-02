//String variableinweb = "test_variable4";
String Get_HTTP_request="";
String Get_HTTP_request_write;
String Get_HTTP_request_read;
//int variableinweb_data= 5;
//int test_variable;
// bString test_variable_string = "test_variable";
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in millisecondschar b_print [0..45];
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
String fields[]={"id","name_pin","in_out","state","trigger"};
unsigned char sizeofarray_fields=4;
unsigned int update_interval=0;
char b_print[30];
char e_print[30];
unsigned char reboot=0;
int index;
void client_setup(){
 index=0;
 for (int i3=2;i3<sizeofarray_fields;i3++){
   for (int i2=0;i2<10;i2++){
    values[i3][i2]=EEPROM.read(index);  
    index++;
    Serial.print(values[i3][i2]); 
   }
}  
  
  if (Ethernet.begin(mac) == 0) {//старт интернета
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);//не получилось, пробуем через IP
  }
  // give the Ethernet shield a second to initialize:
    Serial.println("connecting...");
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  //Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP()); 
 // pinMode(pin[3], OUTPUT);

 
 
 
 } 

  // if you get a connection, report back via serial:
  ///////////////////////////////////////////////////////////////////////////////////

  void httpRequest() {

  client_my.stop();
  if (client_my.connect(server_my, 80)) {
  Serial.println("connected");
    //if (record==1){//если есть на запись в сервер:
    //String actual_state_transmit="";
    //actual_state
    // записываем данные на сервер:
    Get_HTTP_request_write="";
    for (int i=0; i < 10; i++){
      Get_HTTP_request_write.concat("&a_s[");
      Get_HTTP_request_write.concat(i);
      delay(1);
      int port= digitalRead(i);
      Get_HTTP_request_write.concat("]=");
      Get_HTTP_request_write.concat(port);
    }
      Get_HTTP_request_write.concat("&last_update=");
      update_interval=(millis()-lastConnectionTime)/1000L;
      Get_HTTP_request_write.concat(update_interval);
      
    //получили все строки на запись в сервер
  
   if (reboot==0){
   Get_HTTP_request_write="&reboot_milis=";
   Get_HTTP_request_write.concat(millis()/1000L);
   Serial.print(Get_HTTP_request_write);//отладка записи данных на экран
   reboot=1;
   }
  

   Serial.print("\n");

  Get_HTTP_request="GET /index.php?";
  Get_HTTP_request.concat(Get_HTTP_request_write);
  Get_HTTP_request.concat(" HTTP/1.1");
  
  Serial.print(Get_HTTP_request);
  client_my.println(Get_HTTP_request);//}
  client_my.println("Host: www.arduinowebserver.hol.es");
  client_my.println("Connection: close");
  client_my.println("User-Agent: arduino-ethernet");
  client_my.println();

  //закончили соединение


Serial.print (lastConnectionTime+"\n");
  for (int i=0; i < 10; i++){
    for (int i1=2; i1 < sizeofarray_fields; i1++){
 
  //  if (millis() - lastConnectionTime > postingInterval+10L*1000L) {
  //  break;break;
//     Serial.print("FREZING\n");
  //}
   delay(200);
    String b_string="db";b_string.concat(fields[i1]);b_string.concat(i);
//    String b_string="db"+fields[i1]+i+"";
    b_string.toCharArray(b_print, 30);
     finder_fields.find(b_print);  // до чего  
    //finder_fields.findUntil(b_print,"<");  // до чего
    values[i1][i] = finder_fields.getValue(':');//переводим все данные в массив
   //запишем все найденное в длинную строку
//READ_DATA=values[i1][i]

  }
}

index=0;
Get_HTTP_request_read="data recieved:"; 
for (int i3=2;i3<sizeofarray_fields;i3++){
  Get_HTTP_request_read.concat("f:");//поле
  Get_HTTP_request_read.concat(i3);//номер поля
  Get_HTTP_request_read.concat(":");
    for (int i2=0;i2<10;i2++){
    //Get_HTTP_request_read.concat("d[");
    //Get_HTTP_request_read.concat(i2);//данные поля с 0
    //Get_HTTP_request_read.concat("]=");
    Get_HTTP_request_read.concat(values[i3][i2]);
//    to_EEPROM[index]=values[i3][i2];
    EEPROM.write(index, values[i3][i2]);  
    index++;
  }
}
Serial.print("\n");
Serial.print(Get_HTTP_request_read);
Serial.print(values[3][3]+"\n");
    // note the time that the connection was made:
    lastConnectionTime = millis();
   }
  else {
  // if you didn't get a connection to the server:
  Serial.println("connection failed");
  }
}

//void software_Reboot()
//{
  
 // while(1)
 // {
 // }
//}
void client_loop(){
loop_wake();

  for (int i=0;i<10;i++){
    if (values[2][i]==0){
    pinMode(pin[i], OUTPUT);
    }
    if (values[2][i]==1){
    pinMode(pin[i], INPUT);
    }
    if (values[3][i]==0){
    digitalWrite(pin[i], LOW);
    }
    if (values[3][i]==1){
    digitalWrite(pin[i], HIGH);
    }
  }
  

  
 // digitalWrite(pin[3], HIGH);   // turn the LED on (HIGH is the voltage level)
 //delay(1000);               // wait for a second
 // digitalWrite(pin[3], LOW);    // turn the LED off by making the voltage LOW
 // delay(1000);               // wait for a second
  
   if (millis()>100L*1000L){
   //reboot=1;
   }

   if (reboot==2){
   //wdt_enable(WDTO_15MS);
   //
   asm volatile ("  jmp 0");
   void(* resetFunc) (void) = 0;
   resetFunc();
   void software_Reset();
   reboot=0;  
   ;}

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }  
  

}

