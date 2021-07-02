#define PREAMBLE_LED 7
#define DATA_LED 6

void setup()
{
  Serial.begin(115200);
  // Board has the data bit wired to pin 8, and we bridge it to pin 2 to be
  // able to attach an interrupt.
  pinMode(8, INPUT);
  pinMode(2, INPUT);
  pinMode(PREAMBLE_LED, OUTPUT);
  pinMode(DATA_LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), pin2ISR, CHANGE);
  Serial.print("Started listening\n");
}
enum RxState {
    RS_NOSYNC = 0,                      // Receiver is inactive
    RS_PREAMBLE = 1,                    // 50% duty cycle
    RS_DATA = 2,                        // DATA is being received
    RS_COMPLETED = 3                    // Receive complete
};

volatile char rx_state = RS_NOSYNC;

char rx_bit_count = 0;
uint32_t rx_buf[3];

unsigned tx_clock = 0;

unsigned long last_timestamp = 0;
unsigned last_pulse_width = 0;
void pin2ISR()
{
  unsigned long timestamp = micros();
  unsigned long pulse_width = timestamp - last_timestamp;
  int pin = digitalRead(2);

  switch (rx_state) {
  case RS_NOSYNC:
      // "Sync" is a high pulse, folowed by a long low
      if (pin == 1 && pulse_width > 10000 && pulse_width < 50000) {
          rx_state = RS_PREAMBLE;
          tx_clock = last_pulse_width;
      }
      break;
  case RS_PREAMBLE:
      if (pulse_width < 2 * tx_clock) {
           tx_clock = (tx_clock + pulse_width) >> 1;
      } else if (pin == 1 && pulse_width > 1000) {
          // pulse_width was for a long low, switch to receiving data.
          rx_state = RS_DATA;
          rx_bit_count = 0;
          memset(rx_buf, 0, sizeof(rx_buf));
      } else {
          rx_state = RS_NOSYNC;         // Transmission error
      }
      break;
  case RS_DATA:
      if (pin == 1) {
          int first = Classify(last_pulse_width);
          int second = Classify(pulse_width);
          if (first == RB_LONG && second == RB_SHORT) { // Received a 1 bit
              int idx = rx_bit_count / 32;
              rx_buf[idx] >>= 1;
              rx_buf[idx] |= 0x80000000;
              rx_bit_count++;
          }
          else if (first == RB_SHORT && second == RB_LONG) { // Received a 0 bit
              int idx = rx_bit_count / 32;
              rx_buf[idx] >>= 1;
              rx_bit_count++;
          }
          else {                        // invalid pulse combination
              rx_state = RS_NOSYNC;
          }
      }
      if (rx_bit_count >= MAX_BITS) {
          rx_state = RS_COMPLETED;
      }
      break;
  }

  last_timestamp = timestamp;
  last_pulse_width = pulse_width;
}
void loop()
{
    digitalWrite(DATA_LED, rx_state == RS_DATA);
    digitalWrite(PREAMBLE_LED, rx_state == RS_PREAMBLE);

    if (rx_state == RS_COMPLETED) {
        if (rx_bit_count >= MAX_BITS) {
            Hcs200_keycode keycode;
            Hcs200Decode(rx_buf, &keycode);
            Hcs200Print(&keycode);
        }
        rx_state = RS_NOSYNC;
    }
}
