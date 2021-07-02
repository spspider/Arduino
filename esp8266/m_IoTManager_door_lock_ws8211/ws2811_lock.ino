char pin_detector = 14;//d5
char pin_lock_locked = 4;//d2
//char pin_door_locked = 14;
char lock_pin = 13;
unsigned char video_activated = 12;
unsigned char timer_detected_move;
//

//
bool detected_move = false;
unsigned long millis_start;

bool timer_firstly_activated;
bool secondary_detect;

unsigned char timer_video_activated = 0;
unsigned char timer_pause_detector = 5;
bool video_activate;
bool pause_detector;

unsigned char sec_video_activated = 65;

bool lock_pattern_active;
unsigned char timer_lock_pattern = 0;
bool lock_locked;

char sec_lock;
char min_lock;

bool lock_pin_invert = 1;
char open_timer = 3;
bool count_lock_timer;
bool lock_opened = false;
bool lock_timer;
const char* main_dataload =  "{\"from\":[0],\"to\":[40],\"type\":[2],\"dir\":[4],\"col\":[1],\"num\":1,\"sp\":10,\"fd\":200,\"br\":2}";
void setup_lock() {
  pinMode(pin_detector, INPUT_PULLUP);
  pinMode(pin_lock_locked, INPUT_PULLUP);
  //  pinMode(pin_door_locked, INPUT_PULLUP);
  pinMode(video_activated, OUTPUT);

  digitalWrite(lock_pin, 0 ^ lock_pin_invert);

  LoadData(main_dataload);
}
void lock_door() {
  if (countdown_lock == 0) {
    digitalWrite(lock_pin, 0 ^ lock_pin_invert);
    count_lock_timer = false;
    Serial.println("замок закрыт");
    
    lock_timer = 0;
    lock_opened = false;
    //playWavFile("closed.wav");
  }
}
void open_lock() {
  lock_opened = true;
  count_lock_timer = true;
  digitalWrite(lock_pin, 1 ^ lock_pin_invert);
  Serial.println("замок открыт");
  //  playWavFile("opened.wav");
}
void loop_lock() {
  if (digitalRead(pin_detector) == 0) {
    if ((!detected_move) && (!pause_detector)) {
      unsigned char randomPattern = random(0, 13);
      if (!timer_firstly_activated) {
        String dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[" +
                          String(randomPattern, DEC) +
                          "],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":20,\"fd\":80,\"br\":10}";
        const char* data_load_c = dataLoad.c_str();
        //Serial.println(dataLoad);
        LoadData(data_load_c);
      }
      detected_move = true;
      video_activate = true;
      Serial.println("detected move");
    }

  }
  if (digitalRead(pin_lock_locked) == 0) {
    //Serial.println("pin_lock_locked");
    lock_pattern_active = true;
    lock_pattern_activate();
    //sendEmail("замок открыт");
  }
  else if (digitalRead(pin_lock_locked) == 1) {
    //Serial.println("pin_lock_locked");
    lock_locked = false;
  }


}
void lock_pattern_activate() {
  if ((lock_pattern_active) && (timer_lock_pattern == 0) && (!lock_locked)) {
    Serial.println("pin_lock_locked");
    const char* dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[6],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":20,\"fd\":200,\"br\":50,\"r\":1,\"dr\":1}";
    //Serial.println(dataLoad);
    timer_lock_pattern = 2;
    LoadData(dataLoad);
    secondary_detect = false;
    lock_locked = true;
    lock_pattern_active = false;
  }

}
void one_sec_lock() {
  calculate_minutes_fo_open();
  if (timer_lock_pattern > 0) {
    timer_lock_pattern--;
  }

  if (pause_detector) {
    if (timer_pause_detector == 0) {
      pause_detector = false;

      timer_pause_detector = 4;
    }
    timer_pause_detector--;
  }

  if (timer_firstly_activated) {

    if (timer_detected_move > 6) {
      //activated_count++;
      if ((detected_move) && (!secondary_detect)) {
        Serial.println("сработало повторно");
        const char* dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[13],\"dir\":[0],\"col\":[0],\"num\":1,\"sp\":10,\"fd\":50}";
        LoadData(dataLoad);
        secondary_detect = true;
      }
    }
    if (timer_detected_move > 30) {
      //не сработало
      timer_firstly_activated = false;
      timer_detected_move = 0;

      Serial.println("нет повторного срабатывания");
      pause_detector = true;
      timer_pause_detector = 10;
      if (secondary_detect) {
        const char* dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[2],\"dir\":[1],\"col\":[0],\"num\":1,\"sp\":10,\"fd\":50,\"br\":5}";
        LoadData(dataLoad);
      }
      else {
        const char* dataLoad = "{\"from\":[0],\"to\":[40],\"type\":[2],\"dir\":[1],\"col\":[0],\"num\":1,\"sp\":20,\"fd\":50,\"br\":5}";
        LoadData(dataLoad);
      }
      secondary_detect = 0;
      detected_move = false;
    }
    timer_detected_move++;
  }

  if ((detected_move) && (!timer_firstly_activated)) { //1sec
    timer_firstly_activated = true;
    detected_move = false;
    pause_detector = true;
  }
  video_activated_void();

  //if (activated_count)
  //check
   digitalWrite(video_activated, LOW);//
}
void calculate_minutes_fo_open() {
  if (sec_lock > 60) {
    sec_lock = 0;
    min_lock++;
  }
  sec_lock++;
  if (min_lock > 0) {
    if (countdown_lock > 0) {
      countdown_lock--;
      Serial.print("countdown_lock:");
      Serial.println(countdown_lock, DEC);
    } else if (countdown_lock == 0) {
      lock_door();
    }

    min_lock = 0;
  }
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
