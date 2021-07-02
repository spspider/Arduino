const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
unsigned long int millisec_fromstart;
int previous_timings;
void setup() {
  Serial.begin(115200);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), signal_rcv_change, CHANGE);
 // attachInterrupt(digitalPinToInterrupt(interruptPin), signal_rcv_falling, FALLING);
}

void loop() {

  //digitalWrite(ledPin, state);
}

void signal_rcv_change() {
  unsigned long int timing = micros() - millisec_fromstart;
  //if (previous_timings)
  //
  millisec_fromstart = micros();
  Serial.println(timing);
  if (abs(previous_timings - millisec_fromstart) < 200) {

  }
  previous_timings = millisec_fromstart;
}

