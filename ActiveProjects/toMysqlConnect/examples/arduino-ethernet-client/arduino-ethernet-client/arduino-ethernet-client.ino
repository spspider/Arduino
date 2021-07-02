// Ethernet declarations
#include <SPI.h>
#include <Ethernet.h>
 
// the media access control (ethernet hardware) address for the shield:
// MAC For my ethernet shield BC:B1:F3:4A:B4:EA
 byte mac[] = { 0xBC, 0xB1, 0xF3, 0x4A, 0xB4, 0xEA };  
 //the IP address for the shield: 192.168.1.5
 byte ip[] = { 192, 168, 1, 25 };    
 
 //What server to connect to?
//byte server[] = { 209, 85, 137, 94 }; //google.no
byte server[] = { 31,170,165,231 };// brygging.bogetvedt.com

//The location to go to on the server
//make sure to keep HTTP/1.0 at the end, this is telling it what type of file it is
String location = "/~public_html/read.php HTTP/1.1";


EthernetClient Client;



char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?

 
void setup()
{
   Serial.begin(9600);
   Serial.println("Initializing ethernet module");
   Ethernet.begin(mac,ip);
   Serial.println(Ethernet.localIP());
}

void loop()
{
  String pageValue = connectAndRead(); //connect to the server and read the output

  Serial.println(pageValue); //print out the findings.

  delay(5000); //wait 5 seconds before connecting again
}

String connectAndRead()
{
  //connect to the server

  Serial.println("connecting...");

  //port 80 is typical of a www page
  if (Client.connect(server, 80)) {
    Serial.println("connected");
    Client.print("GET ");
    Client.println(location);
    Client.println();

    //Connected - Read the page
    return readPage(); //go and read the output
  }else{
    return "connection failed";
  }
}

String readPage()
{
  //read the page, and capture & return everything between '<' and '>'

  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory

  while(true){

    if (Client.available()) {
      char c = Client.read();

      if (c == '<' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }else if(startRead){

        if(c != '>'){ //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }else{
          //got what we need here! We can disconnect now
          startRead = false;
          Client.stop();
          Client.flush();
          Serial.println("disconnecting.");
          return inString;
        }
      }
    }
  }
}
