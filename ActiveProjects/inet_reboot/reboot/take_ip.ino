void check_ip(){
  ///////////////////////////////////////////////////////////////////
    starttime = millis();
  while (!client_my.available()) 
    { if ((millis() - starttime) > 200000)
       {
         Serial.println("Timeout");
         setup();
          //client_my.stop();
          //client_my.flush(); 
         break;
         
       }
    } 

  if(finder.find("IP Address: "))
   { for (char k = 0; k < 17; k++)
     { c = client_my.read();
       if(c != '<')
       { //Serial.print(c);
         externalIP[k] = c;
       }
       else 
        break;
      }
   }
  Serial.println(externalIP);
////////////////////////////////////////////////////////////////
  }  
  void take_ip(){
    client_my.stop();
    if (client_my.connect(server_checkip,80)) {
    Serial.print("take ip... ");
    client_my.println("GET / HTTP/1.1");
    client_my.println();
    delay(1000);
  }

 if (client_my.connected()) {

  check_ip();
  Serial.println("ip taken... ");

 }

   

  client_my.stop();
  client_my.flush(); 
  }
