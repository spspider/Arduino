//#include <Ethernet.h>
//EthernetClient wclient;
WiFiClient client_my;
char smtp_arr[] = "mail.smtp2go.com"; 
int smtp_port = 2525;

String to_email_addr = "spspider@mail.ru"; // destination email address
String from_email_addr = "spspider95@smtp2go.com"; //source email address

//base64 encoded username and password_email
//Use this site to encode: http://webnet77.com/cgi-bin/helpers/base-64.pl/


String emaillogin = "c3BzcGlkZXI5NUBnbWFpbC5jb20="; //username
String password_email = "NTUwNjQ4Nw=="; //password_email







byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;
 
  if(client_my.connect(smtp_arr,smtp_port) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending hello"));
// replace 1.2.3.4 with your Arduino's ip
  client_my.println("EHLO");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending auth login"));
  client_my.println("auth login");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending User"));
// Change to your base64 encoded user
  client_my.println(emaillogin);
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending password_email"));
// change to your base64 encoded password_email
  client_my.println(password_email);
 
  if(!eRcv()) return 0;
 
// change to your email address (sender)
  Serial.println(F("Sending From"));
     client_my.print("MAIL FROM: <"); // identify sender 
     client_my.print(from_email_addr);
     client_my.println(">");
  if(!eRcv()) return 0;
 
// change to recipient address
  Serial.println(F("Sending To"));
     client_my.print("RCPT TO: <"); // identify recipient 
     client_my.print(to_email_addr);
     client_my.println(">");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending DATA"));
  client_my.println("DATA");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending email"));
 
    // start of email 
    client_my.print("To: ");
    client_my.println(to_email_addr);
    client_my.print("From: ");
    client_my.println(from_email_addr);
    client_my.print("Subject: нажата кнопка");
//    client_my.print(off_time);
//    client_my.println(" секунд\r\n");

    client_my.println("нажата нопка");
 
  client_my.println(".");
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending QUIT"));
  client_my.println("QUIT");
  if(!eRcv()) return 0;
 
  client_my.stop();
 
  Serial.println(F("disconnected"));
 
  return 1;
}
 
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
 
  while(!client_my.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client_my.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
 
  respCode = client_my.peek();
 
  while(client_my.available())
  {  
    thisByte = client_my.read();    
    Serial.write(thisByte);
  }
 
  if(respCode >= '4')
  {
    efail();
    return 0;  
  }
 
  return 1;
}
 
 
void efail()
{
  byte thisByte = 0;
  int loopCount = 0;
 
  client_my.println(F("QUIT"));
 
  while(!client_my.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client_my.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }
 
  while(client_my.available())
  {  
    thisByte = client_my.read();    
    Serial.write(thisByte);
  }
 
  client_my.stop();
 
  Serial.println(F("disconnected"));
}

