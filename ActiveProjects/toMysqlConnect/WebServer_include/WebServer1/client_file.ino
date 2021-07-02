String Get_HTTP_request="";
String Get_HTTP_request_write;
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
   Serial.print(fields[i3]); 
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
  //delay(1000);
  
  // start the Ethernet connection using a fixed IP address and DNS server:
  //Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP()); 
 // pinMode(pin[3], OUTPUT);

 
 
 
 } 

  void httpRequest() {

  client_my.stop();
  if (client_my.connect(server_my, 80)) {

  Serial.println("connected");
    Get_HTTP_request_write="";
    for (int i=0; i < 10; i++){
      Get_HTTP_request_write.concat("&a_s[");
      Get_HTTP_request_write.concat(i);
      //delay(1);
      int port= digitalRead(i);
      Get_HTTP_request_write.concat("]=");
      Get_HTTP_request_write.concat(port);
    }
      Get_HTTP_request_write.concat("&last_update=");
      update_interval=(millis()-lastConnectionTime)/1000L;
      Get_HTTP_request_write.concat(update_interval);
      
    //получили все строки на запись в сервер
  
   if (reboot==0){
   Get_HTTP_request_write.concat("&reboot_milis=");
   Get_HTTP_request_write.concat(millis()/1000L);
   //Serial.print(Get_HTTP_request_write);//отладка записи данных на экран
   reboot=1;
   }
  if (wakeup==2){
  Get_HTTP_request_write.concat("&wakeup=2");
  wakeup=0;
  }

  Serial.print("\n");
  Get_HTTP_request="";
  if (request_now==0){
  Host = "Host: www.arduinowebserver.hol.es";
  Get_HTTP_request="GET /index.php?";
  Get_HTTP_request.concat(Get_HTTP_request_write);
  }
  if (request_now==1){
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
  //delay(1000); 

  //client.stop();
}

void client_loop(){
///////////////////////////////////  
  
if (client_my.connected()) {
    if (client_my.available()) {
if (request_now==0){
    //Serial.print (lastConnectionTime+"\n");
    
  Serial.println("\n");
 // if (client_my.available()) {
     for (int i=0; i < 10; i++){ 
      for (int i1=0; i1 < sizeofarray_fields; i1++){
    //    String b_string="db";b_string.concat(fields[i1]);b_string.concat(i);b_string.concat(":");
        String b_string=fields[i1];b_string.concat(i);b_string.concat(":");
    
        b_string.toCharArray(b_print, 30);
        Serial.println(b_print);
        if(finder_fields.find(b_print)){  // до чего 
          values[i1][i] = finder_fields.getValue();//переводим все данные в массив
          Serial.println(values[i1][i]);
        }
      }
    }
      finder_fields.find("wakeup=");
      wakeup = finder_fields.getValue();//переводим все данные в массив
//  }
}
if (request_now==1){//ищем телефон////////////////////////
        if(finder_fields.getString("android","aea2118",buffer,sizeof(buffer))){
          Serial.println(buffer);}
}
/////выводим для логирования//////////////////////////
index=0;
Get_HTTP_request_read="data:"; 
for (int i3=0;i3<sizeofarray_fields;i3++){
  Get_HTTP_request_read.concat("f:");//поле
  Get_HTTP_request_read.concat(i3);//номер поля
  Get_HTTP_request_read.concat(":");
/////перевод в память EEPROM/////////////////////////////////////////
    for (int i2=0;i2<10;i2++){
    Get_HTTP_request_read.concat(values[i3][i2]);
    EEPROM.write(index, values[i3][i2]);  
    index++;
  }
}
}
/////////////////////////////////////////////////
    Serial.print("\n");
    Serial.print(Get_HTTP_request_read);
    Serial.print(values[3][3]+"\n");
    // note the time that the connection was made:
    lastConnectionTime = millis();
    }  
  
///////////////////////////////////  
loop_wake();

  for (int i=0;i<10;i++){
    if (values[0][i]==0){    pinMode(pin[i], OUTPUT);    }
    if (values[0][i]==1){    pinMode(pin[i], INPUT);    }
 
    if (values[1][i]==0){    digitalWrite(pin[i], LOW);    }
    if (values[1][i]==1){    digitalWrite(pin[i], HIGH);    }
    
  }
  


  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }  
  

}

