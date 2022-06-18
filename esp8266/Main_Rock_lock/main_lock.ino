unsigned long check_interval;
unsigned long check_interval2;

bool count_lock_timer;
unsigned char number_client;
char lock_timer;
int timer_lock_anyway;
char sec_lock;
char min_lock;
bool lock_opened = false;

bool timer_enable_wifi = 0;
char timer_enable_wifi_counting = 0;
void setup_lock() {
  pinMode(lock_pin, OUTPUT);
  digitalWrite(lock_pin, lock_pin_invert);
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
void loop_lock() {
  if (millis() > check_interval2 + 100L ) {
    if (count_lock_timer) {
      lock_timer++;
      //timer_lock_anyway++;
    }

    check_interval = millis();
  }
  if (millis() > check_interval + 1000L ) {
    //client_status();
    if ((open_type == 1) || (open_type == 2)) {
      register_all_mac();
    }
    /*
      if (!register_is_on) {
      check_mac();
      }
      else {
      register_all_mac();
      }
    */
    /////////////////////////
    calculate_minutes_fo_open();
    /////////////////////////////
    check_interval = millis();
    if (count_lock_timer) {
      lock_timer++;
      //timer_lock_anyway++;
    }
    if (timer_enable_wifi) {
      if (timer_enable_wifi_counting > 10) {
        timer_enable_wifi_counting = 0;
        timer_enable_wifi = 0;
        setup_captive();
      }
      timer_enable_wifi_counting++;
    }
  }
  if (lock_timer > open_timer) {
    // if (number_client == 0) {

    lock_door();
    // WiFi.softAPdisconnect (true);
    //Serial.println("замок закрыт,таймер");
    //timer_lock_anyway = 0;
    timer_enable_wifi = true;
    // }
  }
  if ((timer_lock_anyway > count_timer_lock_anyway) && (lock_opened) && (countdown_lock == 0)) {
    lock_door();
    if (!connectAsAp)return;
    WiFi.softAPdisconnect (true);
    Serial.println("замок закрыт,таймер");
    timer_lock_anyway = 0;
    timer_enable_wifi = true;
  }
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
  delay(500);
  Serial.println("замок открыт");
  //  playWavFile("opened.wav");
}



void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
void register_all_mac() {
  number_client = wifi_softap_get_station_num();
  struct station_info *stat_info;
  stat_info = wifi_softap_get_station_info();
  if ( (number_client > 0) && (stat_info != NULL)) {
    String mac_adress = readCommonFiletoJson("mac_adress");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& rootjs = jsonBuffer.parseObject(mac_adress);
    uint8_t num = rootjs["num"];
    //register_is_on = rootjs["reg"];
    num > 254 ? num = 254 : 0;
    unsigned char number_client;

    while (stat_info != NULL) {
      byte array[6] = {stat_info->bssid[0], stat_info->bssid[1], stat_info->bssid[2], stat_info->bssid[3], stat_info->bssid[4], stat_info->bssid[5]};
      char str[32] = "";
      array_to_string(array, 6, str);
      if (register_is_on) {
        strcpy(str_connected_mac, str);
      }
      Serial.print("connected mac adress:");
      Serial.println(str_connected_mac);
      bool found_compare = false;
      DynamicJsonBuffer jsonBuffer_save;
      JsonObject& root = jsonBuffer.createObject();
      JsonArray& mac_js = root.createNestedArray("mac");
      for (uint8_t i = 0; i < num; i++) {
        char mac[32];
        if (rootjs["mac"][i]) {
          strcpy(mac, rootjs["mac"][i]);
          if (strcmp(mac, str) == 0) {
            Serial.println("compare found!");
            compare_found_global = true;
            if (strcmp(str_connected_mac, str) != 0) {
              if (!register_is_on) {
                open_lock();
              }

              strcpy(str_connected_mac, str);
            }

            found_compare = true;
          };
          if (register_is_on) {
            mac_js.add(mac);//добавляем предидущие значения
          }
        }
      }
      if (register_is_on) {//режим регистрации
        if (!found_compare) {
          mac_js.add(str);
          //Serial.println(mac_js);
          //Serial.print("mac_js.size()");
          //Serial.println(mac_js.size());
          num = mac_js.size();
          root["num"] = num;
          String buffer;
          root.printTo(buffer);
          saveCommonFiletoJson("mac_adress", buffer, 1);
          open_lock();
        }
      }
      stat_info = STAILQ_NEXT(stat_info, next);
      // delay(500);
    }
  } else {
    memset(str_connected_mac, 0, sizeof str_connected_mac);
    compare_found_global = false;
  }
}

void client_status() {

  struct station_info *stat_info;


  number_client = wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
  stat_info = wifi_softap_get_station_info();

  // Serial.print(" Total connected_client are = ");
  //Serial.println(number_client, DEC);
  if (number_client > 0) {
    Serial.print("wifi_softap_get_station_info();:");
    Serial.println(WiFi.softAPgetStationNum());


    Serial.print("WiFi.status():");
    Serial.println(WiFi.status());
    if (status == WL_CONNECT_FAILED) {

      Serial.print("!!!Connect Failed");
    }
    /*
      if ((!count_lock_timer) && (WiFi.softAPPSK() == String(softAP_password))) {

      uint8_t i = 1;
      while (stat_info != NULL) {

        String Mac_adress;
        for (uint8_t i1 = 0; i1 < 5; i1++) {
          Mac_adress += (String(stat_info->bssid[i1], HEX));
        }
        stat_info = STAILQ_NEXT(stat_info, next);
        i++;
        Serial.println(Mac_adress);
      }


      //open_lock();

      //WiFi.disconnect();
      }
    */
  } else {
    station_connected = false;
  }

  /*
    while (stat_info != NULL) {

      IPaddress = &stat_info->ip;
      address = IPaddress->addr;

      Serial.print("client= ");

      Serial.print(i);
      Serial.print(" ip adress is = ");
      Serial.print((address));
      Serial.print(" with mac adress is = ");

      Serial.print(stat_info->bssid[0], HEX);
      Serial.print(stat_info->bssid[1], HEX);
      Serial.print(stat_info->bssid[2], HEX);
      Serial.print(stat_info->bssid[3], HEX);
      Serial.print(stat_info->bssid[4], HEX);
      Serial.print(stat_info->bssid[5], HEX);

      stat_info = STAILQ_NEXT(stat_info, next);
      i++;
      Serial.println();

    }
  */
}
