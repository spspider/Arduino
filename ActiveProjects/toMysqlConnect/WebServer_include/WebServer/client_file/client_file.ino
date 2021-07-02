int Number_fields=1;
int length_check;
char record=0;
String variableinweb = "test_variable4";
String Get_HTTP_request="";
String Get_HTTP_request_write;
int variableinweb_data= 5;
int test_variable;
String test_variable_string = "test_variable";
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
String fields[]={"id","name_pin","in_out","state","trigger"};
int sizeofarray_fields=4;
char b_print[30];
char e_print[30];
unsigned char reboot=0;
void client_setup(){
  
  
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
  void(* resetFunc) (void) = 0;
  void software_Reset();
  void httpRequest() {

  client_my.stop();
  if (client_my.connect(server_my, 80)) {
  Serial.println("connected");
    //if (record==1){//если есть на запись в сервер:
    //String actual_state_transmit="";
    //actual_state
    Get_HTTP_request_write="";
    for (int i=0; i < 10; i++){
      Get_HTTP_request_write += "&a_s[";
      Get_HTTP_request_write.concat(i);
      delay(1);
      int port= digitalRead(i);
      Get_HTTP_request_write+="]=";
      Get_HTTP_request_write.concat(port);
    }
    
   if (reboot==1){
   Get_HTTP_request_write="reboot_milis=";
   Get_HTTP_request_write.concat(millis());
   Serial.print(Get_HTTP_request_write);
   reboot=2;
   }
  

   Serial.print("\n");

   Get_HTTP_request="GET /index.php?"+Get_HTTP_request_write+" HTTP/1.1";

  
  client_my.println(Get_HTTP_request);
  client_my.println("Host: www.arduinowebserver.hol.es");
  client_my.println("Connection: close");
  client_my.println("User-Agent: arduino-ethernet");
  client_my.println();
  Serial.print(Get_HTTP_request);
  //закончили соединение
  
 ///узнаем кол-во записей//////////////////////////////////////////////
 //Number_fields=10;
//finder_numfields.findUntil("Number_fields:",":number");  // до чего
// Number_fields = finder_numfields.getValue();

//Serial.print("Number_fields:");
//Serial.print(Number_fields);Serial.println("\n");

Serial.print (lastConnectionTime+"\n");
  for (int i=0; i < 10; i++){
    for (int i1=0; i1 < sizeofarray_fields; i1++){
 
  //  if (millis() - lastConnectionTime > postingInterval+10L*1000L) {
  //  break;break;
//     Serial.print("FREZING\n");
  //}
   delay(200);
    String b_string="db"+fields[i1]+i+"";
    //String e_string="ve"+fields[i1]+i;
    //e_string=":variableend";
    b_string.toCharArray(b_print, 30);
    //e_string.toCharArray(e_print, 30);
   // Serial.print(b_print);
     finder_fields.find(b_print);  // до чего  
    //finder_fields.findUntil(b_print,"<");  // до чего
    values[i1][i] = finder_fields.getValue(':');//переводим все данные в массив
   // Serial.print("i1:");
   // Serial.print(i1);
   // Serial.print("i:");
   // Serial.print(i);
   // Serial.print("val:");
   // Serial.print(values[i1][i]);
   // Serial.println(":\n");
    /////////////////////////////////////////////////////////////////  
  
  }
}
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
  
  
  
  
 // int i=0;
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
// for (int i2=0;i2<Number_fields;i2++){
  // for (int i3=0;i3<sizeofarray_fields;i3++){
   // Serial.print("i1:");
   // Serial.print(i3);
   // Serial.print("i:");
   // Serial.print(i2);
   // Serial.print("val:");
  //  Serial.print(values[i3][i2]);
   // Serial.println(":\n");
 //  }
// }
//Serial.print(values[3][3]);
  
 // digitalWrite(pin[3], HIGH);   // turn the LED on (HIGH is the voltage level)
 //delay(1000);               // wait for a second
 // digitalWrite(pin[3], LOW);    // turn the LED off by making the voltage LOW
 // delay(1000);               // wait for a second
  
if (millis()>50L*1000L){
  reboot=1;

}
  if (reboot==2){
wdt_enable(WDTO_15MS);
resetFunc();
asm volatile ("  jmp 0");  
;}
//if reoot
  if (millis() - lastConnectionTime > postingInterval) {
    //software_Reboot();
    httpRequest();
    //wdt_enable(WDTO_8S);
    //reset;
  }  
  

}

