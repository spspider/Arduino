#include  <TimerOne.h>  //TimerOne.h

#define CLIENT_ID       "Dimmer"
#define PUBLISH_DELAY   3000



bool startsend = HIGH;

volatile int i = 0;             // Variable to use as a counter volatile as it is in an interrupt
volatile boolean zero_cross = 0; // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin1 = 4;// Output to Opto Triac
int dim1 = 0;// Dimming level (0-100)  0 = on, 100 = 0ff
char pinInterrupt=0; //PD2
int inc = 1;                    // counting up or down, 1=up, -1=down
int freqStep = 100;

long previousMillis;

void zero_cross_detect() {
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i = 0;
  digitalWrite(AC_pin1, LOW);       // turn off TRIAC (and AC)
}

// Turn on the TRIAC at the appropriate time
void dim_check() {
  if (zero_cross == true) {
    if (i >= dim1) {
      digitalWrite(AC_pin1, HIGH); // turn on light
      i = 0; // reset time step counter
      zero_cross = false; //reset zero cross detection
    }
    else {
      i++; // increment time step counter
    }
  }
}

void setup_zero_cross() {

  attachInterrupt(pinInterrupt, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);
  pinMode(4, OUTPUT);

  ip = String (Ethernet.localIP()[0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[3]);
  //Serial.println(ip);
}

void loop_zero_cross() {
  // it's time to send new data?
  if (millis() - previousMillis > PUBLISH_DELAY) {
    sendData();
    previousMillis = millis();
  }
  Serial.print("dim1  = ");
  Serial.println(dim1);
}

void sendData() {
  char msgBuffer[20];
  if (PubClient.connect(CLIENT_ID)) {
    PubClient.subscribe("home/br/sb");
    if (startsend) {
      PubClient.publish("home/br/nb/ip", ip.c_str());
      startsend = LOW;
    }
  }
}
/*
void callback(char* topic, byte* payload, unsigned int length) {
  char msgBuffer[20];
  payload[length] = '\0';            // terminate string with '0'
  String strPayload = String((char*)payload);  // convert to string
  Serial.print("strPayload =  ");
  Serial.println(strPayload); //can use this if using longer southbound topics
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");//MQTT_BROKER
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(payload[0]);

  dim1 = strPayload.toInt();

}
*/
