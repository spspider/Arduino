char pin_detector = 13;
char pin_lock_locked = 12;
char pin_door_locked = 14;
unsigned char video_activated = 16;
unsigned char timer_detected_move;

bool detected_move = false;
unsigned long millis_start;

bool timer_firstly_activated;
bool secondary_detect;

unsigned char timer_video_activated = 0;
bool video_activate;

unsigned char sec_video_activated = 10;

bool lock_pattern_active;
unsigned char timer_lock_pattern = 0;
bool lock_locked;

const char* main_dataload =  "{\"from\":[0],\"to\":[40],\"type\":[2],\"dir\":[4],\"col\":[1],\"num\":1,\"sp\":10,\"fd\":200,\"br\":2}";
void setup_lock() {
  pinMode(pin_detector, INPUT_PULLUP);
  pinMode(pin_lock_locked, INPUT_PULLUP);
  pinMode(pin_door_locked, INPUT_PULLUP);
  pinMode(video_activated, OUTPUT);
  LoadData(main_dataload);
}

void loop_lock() {
  if (digitalRead(pin_detector) == 0) {
    if (!detected_move) {
      unsigned char randomPattern = random(0, 13);
      String dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[" +
                        String(randomPattern, DEC) +
                        "],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":20,\"fd\":80,\"br\":10}";
      const char* data_load_c = dataLoad.c_str();
      //Serial.println(dataLoad);
      LoadData(data_load_c);
      detected_move = true;
      video_activate = true;
      Serial.println("detected move");
    }

  }
  if (digitalRead(pin_lock_locked) == 0) {
    //Serial.println("pin_lock_locked");
    lock_pattern_active = true;
  }
  else if (digitalRead(pin_lock_locked) == 1) {
    //Serial.println("pin_lock_locked");
    lock_locked = false;
  }
  lock_pattern_activate();
}
void lock_pattern_activate() {
  if ((lock_pattern_active) && (timer_lock_pattern == 0) && (!lock_locked)) {
    Serial.println("pin_lock_locked");
    timer_lock_pattern = 2;
    const char* dataLoad = "{\"from\":[0],\"to\":[20],\"type\":[6],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":20,\"fd\":200,\"br\":50,\"r\":1,\"dr\":1}";
    //Serial.println(dataLoad);
    LoadData(dataLoad);
    secondary_detect=false;
    lock_locked = true;
    lock_pattern_active = false;
  }
  if (timer_lock_pattern != 0) {
    if (timer_lock_pattern == 20 - 2) {

    }
  }

}
void one_sec_lock() {

  if (timer_lock_pattern > 0) {
    timer_lock_pattern--;
  }

  if (timer_firstly_activated) {
    if (timer_detected_move > 1) {
      //activated_count++;
      if ((detected_move) && (!secondary_detect)) {
        Serial.println("сработало повторно");
        const char* dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[13],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":10,\"fd\":50}";
        LoadData(dataLoad);
        secondary_detect = true;
      }
    }
    if ((timer_detected_move > 30)) {
      //не сработало
      timer_firstly_activated = false;
      timer_detected_move = 0;
      secondary_detect = 0;
      Serial.println("нет повторного срабатывания");
      LoadData(main_dataload);
      detected_move = false;
    }
    timer_detected_move++;
  }
  if ((detected_move) && (!timer_firstly_activated)) { //1sec
    timer_firstly_activated = true;
    detected_move = false;

  }
  video_activated_void();

  //if (activated_count)
  //check
}
void   video_activated_void() {
  if ((video_activate) && (timer_video_activated == 0)) {
    timer_video_activated = sec_video_activated;
    digitalWrite(video_activated, HIGH);
    Serial.println("HIGH");
    video_activate = false;
  }
  if (timer_video_activated != 0) {
    if (timer_video_activated == sec_video_activated - 2) {
      digitalWrite(video_activated, LOW);
      Serial.println("LOW");
    }
    timer_video_activated--;
  }
}
