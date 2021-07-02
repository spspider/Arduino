String Get_HTTP_request="";
String Get_HTTP_request_write;
//String Get_HTTP_request_write_analog;
String Get_HTTP_request_read;
String Host,Android;
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in millisecondschar b_print [0..45];
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
//String fields[]={"id","name_pin","in_out","state","pwm_st","pwm_len"};
String fields[]={"I","S","P","L"};

int sizeofarray_fields=4;
int update_interval=0;
char b_print[30];
int index,wakeup=0,request_now=0,reboot=0;
char buffer[8];

void client_setup(){
 index=0;
 for (int i3=0;i3<sizeofarray_fields;i3++){
   Serial.print("\n"); 
   Serial.print(fields[i3]); 
   for (int i2=0;i2<14;i2++){
    //values[i3][i2]=EEPROM.read(index);
    if (values[1][i2]==2){values[1][i2]=0;}
    index++;
    Serial.print(values[i3][i2]); 
   }
   
}  
  
  if (Ethernet.begin(mac) == 0) {//старт интернета
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);//не получилось, пробуем через IP
  }
  // give the Ethernet shield a second to initialize:
  //delay (1000);
  Serial.println("connecting...");
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP()); 

 
 
 
 } 

  void httpRequest() {

  client_my.stop();
 //delay(1000);

  if (client_my.connect(server_my,80)) {

  Serial.println("connected");
  Get_HTTP_request_write="";
  if (request_now==0){
    
    for (int i=0; i < 14; i++){
    Get_HTTP_request_write.concat("&a_s[");
    Get_HTTP_request_write.concat(i);
    //delay(1);
    unsigned char port= digitalRead(i);
    Get_HTTP_request_write.concat("]=");
    Get_HTTP_request_write.concat(port);
    }
  }
//  if (request_now==1){
//    Get_HTTP_request_write_analog="";
//    for (int i=0; i < 6; i++){
//    unsigned int val = analogRead(i); 
//    //delay(1000);
//    Get_HTTP_request_write.concat("&a_r[");
//    Get_HTTP_request_write.concat(i);
//   Get_HTTP_request_write.concat("]=");
//    Get_HTTP_request_write.concat(val);
//    }
//  }    
      Get_HTTP_request_write.concat("&last_update=");
      Get_HTTP_request_write.concat("11");
      
    //получили все строки на запись в сервер
  
   if (reboot==0){
   Get_HTTP_request_write.concat("&reboot_milis=");
   Get_HTTP_request_write.concat("10");
   //Serial.print(Get_HTTP_request_write);//отладка записи данных на экран
   reboot=1;
   }
  if (wakeup==2){
  Get_HTTP_request_write.concat("&wakeup=2");
  wakeup=0;
  }

  Serial.print("\n");
  Get_HTTP_request="";
  if ((request_now==0)||(request_now==1)){
  Host = "Host: www.arduinowebserver.hol.es";
  Get_HTTP_request="GET /index.php?";
  Get_HTTP_request.concat(Get_HTTP_request_write);

  }
  if (request_now==2){
  Host = "Host: 192.168.1.1";
  Get_HTTP_request="GET /userRpm/AssignedIpAddrListRpm.htm";

  }
  Get_HTTP_request.concat(" HTTP/1.1");
  Serial.print(Get_HTTP_request);
  client_my.println(Get_HTTP_request);
  client_my.println(Host);
  client_my.println("Connection: close");
  client_my.println("User-Agent: arduino-ethernet");
  client_my.println();

  //закончили соединение
   }
  else {
  // if you didn't get a connection to the server:
  Serial.println("connection failed");
  }
  
  client_file_read();
//client_file_read_coded();
  //request_now++;
 // if (request_now>1){request_now=0;}
}

void client_loop(){
  //if (client_my.available()) {
   // Serial.println("I");

 // }
loop_wake();

  for (int i=0;i<14;i++){
    if (values[0][i]==0){    pinMode(pin[i], OUTPUT);    }
    if (values[0][i]==1){    pinMode(pin[i], INPUT);    }
 
    if (values[1][i]==0){    digitalWrite(pin[i], LOW);    }
    if (values[1][i]==1){    digitalWrite(pin[i], HIGH);    }
    
  }
  


  if (millis() - lastConnectionTime > postingInterval) {
        httpRequest();
  }  
  

}

