// ESP8266 Arduino sketch to control a variac with a stepper motor
// GNU GPL
// NopHead
// hydraraptor.blogspot.com
//
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ArduinoOTA.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include "WiFiManager.h"
#include <ESP8266mDNS.h>
#include "esp8266_peri.h"
#include "ets_sys.h"

const int RED_LED = 4;          // GPIO pins
const int GREEN_LED = 5;        // Bi colour LED
const int STEP = 0;             // Stepper driver STEP pin
const int EN = 2;               // Stepper driver enable pin
const int SYNC = 12;            // Display chip CS used for synchonization
const int DIR = 15;             // Stepper driver enable

ESP8266WebServer server ( 80 );

int target_v = 230;                 // target voltage from web form
int speed = 50;                     // speed in degrees per second
int gain = 20;                      // feadback gain, microsteps per volt
float deadband = 0.5;               // deaband to avoid continual movement and wear
bool manual = true;                 // when true the motor is disabled

const int microsteps = 16;          // configuration of the stepper driver
const int big_teeth = 78;           // teeth on the big pulley
const int small_teeth = 16;         // teeth on the motor pulley
const float steps_per_rev = 200.0 * microsteps * big_teeth / small_teeth;
int position = 0;                   // current motor postion relative to where it started

const int length = 12;              // length of packet minus the first byte
uint8_t data[length];               // raw data received
volatile bool data_ready = false;   // set by HSPI interrupt handler when data is received
volatile bool new_readings = false; // set by sync pin interrupt when readings are extracted from raw data
volatile uint32_t samples = 0;      // count of samples received

int   voltint = 0, currentint = 0, wattint = 0;       // raw values
float voltfloat = 0, currentfloat = 0, wattfloat = 0; // real unit values

float voltscalefactor = 0.0307162746 / 256;           // calibration constants vary from device to device
float currentscalefactor = 1.553 / 256000;   
float wattscalefactor = 15.49521794871 / 2560;  

long speed2us(int speed) {                            // convert speed in degrees per second to setp delay in microseconds
  float steps_per_second = steps_per_rev * speed / 360.0;
  return 1000000 / steps_per_second;
}

void spin(int speed, int steps) {                     // step the motor
    digitalWrite(GREEN_LED, 1);
    digitalWrite(DIR, steps < 0);
    digitalWrite(EN, 0);
    delay(1);
    long uS = speed2us(speed);
    position += steps;
    steps = abs(steps);
    while(steps--) {
         digitalWrite(STEP, 1);
         ESP.wdtFeed();
         digitalWrite(STEP, 0);
         delayMicroseconds(uS);
    }
}

const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.5, maximum-scale = 2.0, user-scalable=1", charset="UTF-8"/>
    <title>Variac control</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
    </style>
  </head>
  <body>
    <h1>Variac Control</h1>
    <form action="/" method="post">
      <table>
         <tr><td>Voltage:</td> <td><input type="text" name="voltage" value="%d" size="3" maxlength="3"></td></tr>
         <tr><td>Speed:</td>   <td><input type="text" name="speed" value="%d" size="3" maxlength="3"></td></tr>
         <tr><td>Gain:</td>    <td><input type="text" name="gain" value="%d" size="3" maxlength="3"></td></tr>
         <tr><td>Deadband:</td><td><input type="text" name="deadband" value="%3.1f" size="3" maxlength="3"></td></tr>
         <tr><td><button type="submit" name="manual">%s</button></td><td><button type="submit" name="set">%s</button></td></tr>
      </table>
      <br/><div>Readings: <pre style="display:inline"><span id = "readings">---</span></pre></div>
    </form>
    <script>
    setInterval(function() {
      // Call a function repetatively with 2 Second interval
      getData();
    }, 200); //200ms update rate
     
    function getData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("readings").innerHTML =
          this.responseText;
        }
      };
      xhttp.open("GET", "readings", true);
      xhttp.send();
    }
    </script>
  </body>
</html>
)=====";

void mainPage() {
  char temp[100 + sizeof MAIN_page];
  snprintf_P ( temp, sizeof temp, MAIN_page, target_v, speed, gain, deadband,
    manual ? "<strong>MANUAL</strong>" : "MANUAL", 
    manual ? "SET" : "<strong>SET</strong>" );
  server.send ( 200, "text/html", temp );
}

int clip(int x, int low, int high) {
  if(x < low)
    return low;
  if(x > high)
    return high;
  return x;
}
  
void handleSettings() {
  if(server.hasArg("speed"))
    speed = clip(server.arg("speed").toInt(), 1, 100);
   
  if(server.hasArg("voltage"))
    target_v =clip(server.arg("voltage").toInt(), 1, 260);

  if(server.hasArg("gain"))
     gain = clip(server.arg("gain").toInt(), 0, 100);

  if(server.hasArg("deadband"))
     deadband = server.arg("deadband").toFloat();

  if(server.hasArg("manual")) {
     if(manual)
        position = 0;
     manual = true;
     digitalWrite(EN, 1);
  }

  if(server.hasArg("set"))
     manual = false;
}

void handleRoot() {
  handleSettings();
  mainPage();
}

void handleReadings() {
  handleSettings();
  char temp[100];
  snprintf(temp, sizeof temp, "%5.1fV %4.3fA %5.1fW Pos %5d", voltfloat, currentfloat, wattfloat, position);
  //snprintf(temp, sizeof temp, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", data[0], data[1],data[2], data[3], data[4], data[5], data[6], data[7],data[8], data[9], data[10], data[11]);
  server.send(200, "text/plane", temp); //Send readings to client ajax request
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  Serial.println(message);
}

void ICACHE_RAM_ATTR sync_isr() {                           // Sync pin interrupt on falling edge
  SPI1S |= SPISSRES;                                        // Reset HSPI slave
  SPI1S &= ~SPISSRES;
  SPI1CMD = SPICMDUSR;                                      // Start HSPI slave
  static bool glitch = false;
  if(data_ready) {                                          // If a data has been received by the HSPI
    data_ready = false;
    int volts = (data[0] << 16) + (data[1] << 8) + data[2]; // assemble 24 bits
    if(volts > 5 * voltint / 8 || glitch) {                 // reject samples that have been shifted right by noise on the clock line?
      glitch = false;                                       // sample accepted
      voltint = volts;
      currentint = (data[4] << 16) + (data[5] << 8) + data[6];
      wattint    = (data[8] << 16) + (data[9] << 8) + data[10];
      if(wattint > 0xFF0000)                                // sometimes goes slightly negative with no load
         wattint = 0;
      ++samples;
      new_readings = true;                                  // new raw readings ready
    }
    else
      glitch = true;                                        // only reject one sample
  }
}

void ICACHE_RAM_ATTR _hspi_slave_isr_handler(void *) {
    uint32_t istatus = SPIIR;
    if(istatus & (1 << SPII1)) {      //SPI1 ISR
        uint32_t status = SPI1S;
        SPI1S &= ~(0x3E0);            //disable interrupts
        SPI1S |= SPISSRES;            //reset
        SPI1S &= ~(0x1F);             //clear interrupts
        SPI1S |= (0x3E0);             //enable interrupts

        if(status & SPISWBIS) {
            uint8_t *p = data;
            for(int i = 0; i < length / 4; i++) {
                uint32_t dword = SPI1W(i);
                *p++ = dword;
                *p++ = dword >> 8;
                *p++ = dword >> 16;
                *p++ = dword >> 24;
            }
            data_ready = true;
        }
    } else if(istatus & (1 << SPII0)) { //SPI0 ISR
        SPI0S &= ~(0x3ff);//clear SPI ISR
    } else if(istatus & (1 << SPII2)) {} //I2S ISR
}

void hspi_slave_begin() {
    pinMode(SCK, SPECIAL);                  // Both inputs in slave mode
    pinMode(MOSI, SPECIAL);
    SPI1C = 0;                              // SPI_CTRL_REG MSB first, single bit data mode.
    SPI1S = SPISE | SPISBE | SPISCD | 0x3E0;// SPI_SLAVE_REG, set slave mode, WR/RD BUF enable, CMD define, enable interrupts
    SPI1U = SPIUSSE;                        // SPI_USER_REG.  SPI_CK_I_EDGE
    SPI1CLK = 0;                            // SPI_CLOCK_REG
    SPI1U1 = 7 << SPILADDR;                 // SPI_USER1_REG, set address length to 8 bits
    SPI1U2 = 7 << SPILCOMMAND;              // SPI_USER2_REG, set command length to 8 bits
    SPI1S1 = (length * 8 - 1) << SPIS1LBUF; // SPI_SLAVE1_REG, SPI_SLV_BUF_BITLEN = 12 bytes
    SPI1S3 = 0xF1F200F3;                    // SPI_SLAVE3_REG,, Define command 0 to be write buffer, others something doesn't match
    SPI1P = 1 << 19;                        // SPI_PIN_REG, Clock idle high, seems to cause contension on the clock pin if set to idle low.
    ETS_SPI_INTR_ATTACH(_hspi_slave_isr_handler, 0);
    ETS_SPI_INTR_ENABLE();
}

void hspi_slave_end() {
    SPI1S |= SPISSRES;
    SPI1CMD = 0;
    pinMode(SCK, INPUT);                  // Return to inputs to avoid glicthing the PZEM-021 during reset
    pinMode(MOSI, INPUT);
    digitalWrite (GREEN_LED, 1);
    digitalWrite (RED_LED, 1);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Variac starting");
  
  pinMode(GREEN_LED, OUTPUT); 
  pinMode(RED_LED, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  digitalWrite(RED_LED, 0);
  digitalWrite(GREEN_LED, 0);
  digitalWrite(STEP, 0);
  digitalWrite(DIR,0);
  digitalWrite(EN,1);
  
  //WiFi.disconnect();
  const char *hostname = "variac";
  digitalWrite ( RED_LED, 1 );
  WiFi.hostname(hostname);
  WiFiManager wifiManager;
  wifiManager.autoConnect(hostname, NULL); // Ap name same as host name
  digitalWrite( RED_LED, 0 );
  digitalWrite( GREEN_LED, 1); 
  if ( MDNS.begin(hostname) )
    Serial.println ( "MDNS responder started" );
  
  server.on( "/", handleRoot );
  server.on("/readings", handleReadings );
  server.onNotFound( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  attachInterrupt(digitalPinToInterrupt(SYNC), sync_isr, FALLING);
  hspi_slave_begin();
  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
    // Stop the interrupts
    detachInterrupt(digitalPinToInterrupt(SYNC));
    hspi_slave_end();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", 100 * progress / total);
    static bool toggle;         // toggle both leds so they flicker orange during update
    toggle = !toggle;
    digitalWrite (GREEN_LED, toggle);
    digitalWrite (RED_LED, toggle);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite ( GREEN_LED, !!(millis() & 512) );           // flash the green LED
  server.handleClient();                                    // run web server
  if(new_readings) {                                        // set by interrupt when data received
    voltfloat = voltint * voltscalefactor;                  // convert to real units
    currentfloat = currentint * currentscalefactor;
    wattfloat = wattint * wattscalefactor;
    Serial.printf("V%5.1f, A%4.2f, W%5.1f, Pos%5d", voltfloat, currentfloat, wattfloat, position);
    for(int i = 0; i < length; ++i)
      Serial.printf(" %02X", data[i]);
    Serial.println();
    new_readings = false;
  }
  if(samples > 1) {                                         // update motor when had more than two samples since last moved
    float error = target_v - voltfloat;
    if(fabs(error) > deadband / 2 && !manual) {
       spin(speed, error * gain);
       samples = 0;
    }
  }
  ArduinoOTA.handle();
}
