

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("start");
  setup_ws2811();
 // setup_lock();
}
void loop() {
  loop_ws2811();
  //loop_lock();
}
