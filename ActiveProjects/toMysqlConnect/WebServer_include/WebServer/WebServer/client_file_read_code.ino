void client_file_read_coded(){

  //delay(1000); 
if (client_my.connected()) {

if ((request_now==0)||(request_now==1)){
 
        char readString[101];
        const int READ_STRING_SIZE = sizeof(readString) - 1;
        int readIndex = 0;
        while (client_my.connected()) {
            if (client_my.available()) {
                char c = client_my.read();
                if (readIndex < READ_STRING_SIZE) {
                    readString[readIndex++] = c;
                }

                if (c == '\n') {
                    // Terminate the read string
                    readString[readIndex] = 0;
                    Serial.println(readString);

                    // Prepare for next reeading
                    readIndex = 0;

                    //client.println("HTTP/1.1 200 OK");
                     for (char i = 0; i < strlen(readString); i++)
                        readString[i] = toupper(readString[i]);

                    bool b = false;

                    int blue;

                    // Look for 'B' value
                    char* bb = strchr(readString, 'B');
                    if (bb != 0 && isdigit(bb[1])) {
                        blue = atoi(&bb[1]);
                        Serial.print("CHANGING BLUE TO: ");
                        //delay(500);
                        Serial.println(blue);
                        b = true;
                    }
                    // Same for 'G' and 'R' value
                    

                }
                    
            }
        }
}        

index=0;
Get_HTTP_request_read="data:"; 

/////////////////////////////////////////////////
    Serial.print("\n");
//    Serial.print(Get_HTTP_request_read);
    //Serial.print(values[3][3]+"\n");
    lastConnectionTime = millis();
    //client_my.stop();
    }

}


