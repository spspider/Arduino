char* delimeter(char* str, char* separator, char num) {
  //printf ("%s\n","ok");
  char* pch;
  pch = strtok (str, separator);
  uint8_t i;
  while (pch != NULL)
  {

    //printf ("%s\n",pch);
    if (i == num) {
      break;
    }
    pch = strtok (NULL, separator);
    i++;
  }
  return pch;
}
void CheckInternet(String request) {
  String respond = getHttp(request);
  if (respond == "fail") { //интернета нет
    Serial.println("Интернета нет");
    relayRouter();
  } else { //интернет есть
    DynamicJsonBuffer jsonBuffer;
    JsonObject& rootjs = jsonBuffer.parseObject(respond);

    //"2018-08-23T07:43";
    String currentDateTime = rootjs["currentDateTime"];
    if (timeStatus() == timeNotSet) {
      setTime(
        currentDateTime.substring(11, 13).toInt() + timezone,
        currentDateTime.substring(14, 16).toInt(),
        0,
        currentDateTime.substring(8, 10).toInt(),
        currentDateTime.substring(5, 7).toInt(),
        currentDateTime.substring(0, 4).toInt()
      );
      //setup_alarm();
    } else {
      timezone = hour() - currentDateTime.substring(11, 13).toInt();
      Serial.println("timezone:" + String(timezone, DEC));
    }
    Serial.println("Интернет есть");
  }
}
/*
 /* strtok example 
#include <string.h>
#include <stdio.h>
char* delimeter(char* str, char* separator, char num) {
  //printf ("%s\n","ok");
  char* pch;
  pch = strtok (str, separator);
  __uint8_t i;
  while (pch != NULL)
  {

    //printf ("%s\n",pch);
    if (i == num) {
      break;
    }
    pch = strtok (NULL, separator);
    i++;
  }
  return pch;
}

int main () {
   char str[80] = "This is - www.tutorialspoint.com - website";
   const char s[2] = "-";
   char *token;
   

   token = strtok(str, s);
   
   while( token != NULL ) {
      printf( " %s\n", token );
    
      token = strtok(NULL, s);
   }
   
  //char buff[80]; strcpy (buff, "ok/ok1/ok2"); printf("%s",delimeter(buff,"/",2));
    char buff1[80];strcpy (buff1, "ok/ok1/ok2"); printf("%s",delimeter(buff1,"/",0));
//   char buff2[80];strcpy (buff2, "ok/ok1/ok2"); printf("%s",delimeter(buff2,"/",2));
    
   return(0);
}
 */
