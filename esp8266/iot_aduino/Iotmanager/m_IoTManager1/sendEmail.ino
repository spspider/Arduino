//#include <Ethernet.h>
//EthernetClient wclient;
//WiFiClient client_my;








byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;
 
  if(wclient.connect(smtp_arr,smtp_port) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending hello"));
// replace 1.2.3.4 with your Arduino's ip
  wclient.println("EHLO");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending auth login"));
  wclient.println("auth login");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending User"));
// Change to your base64 encoded user
  wclient.println(emaillogin);
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending password_email"));
// change to your base64 encoded password_email
  wclient.println(password_email);
 
  if(!eRcv()) return 0;
 
// change to your email address (sender)
  Serial.println(F("Sending From"));
     wclient.print("MAIL FROM: <"); // identify sender 
     wclient.print(from_email_addr);
     wclient.println(">");
  if(!eRcv()) return 0;
 
// change to recipient address
  Serial.println(F("Sending To"));
     wclient.print("RCPT TO: <"); // identify recipient 
     wclient.print(to_email_addr);
     wclient.println(">");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending DATA"));
  wclient.println("DATA");
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending email"));
 
    // start of email 
    wclient.print("To: ");
    wclient.println(to_email_addr);
    wclient.print("From: ");
    wclient.println(from_email_addr);
    wclient.print("Subject: нажата кнопка");
//    wclient.print(off_time);
//    wclient.println(" секунд\r\n");

    wclient.println("нажата нопка");
 
  wclient.println(".");
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending QUIT"));
  wclient.println("QUIT");
  if(!eRcv()) return 0;
 
  wclient.stop();
 
  Serial.println(F("disconnected"));
 
  return 1;
}
 
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
 
  while(!wclient.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      wclient.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
 
  respCode = wclient.peek();
 
  while(wclient.available())
  {  
    thisByte = wclient.read();    
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
 
  wclient.println(F("QUIT"));
 
  while(!wclient.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      wclient.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }
 
  while(wclient.available())
  {  
    thisByte = wclient.read();    
    Serial.write(thisByte);
  }
 
  wclient.stop();
 
  Serial.println(F("disconnected"));
}

