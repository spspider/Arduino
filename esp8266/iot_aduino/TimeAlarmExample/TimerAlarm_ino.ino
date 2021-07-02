/*
   TimeAlarmExample.pde

   This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
   and simulates turning lights on at night and off in the morning
   A weekly timer is set for Saturdays at 8:30:30

   A timer is called every 15 seconds
   Another timer is called once only after 10 seconds

   At startup the time is set to Jan 1 2011  8:29 am
*/
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <stdio.h>
#include <string.h>

const unsigned int Numbers = 5;
const unsigned int Condition = 5;
AlarmId alalrmId;

int tID_a[Condition][Numbers];
int type_a[Condition][Numbers];
unsigned int timer_a[Condition][Numbers];
int timerType_a[Condition][Numbers];//надо переделать в int
int act_a[Condition][Numbers];
String actBtn_a[Condition][Numbers];
//char actBtn_a_ch[Condition][Numbers][50];
int actOn_a[Condition][Numbers];//надо переделать в int
int NumberIDs[Condition];

bool alarm_is_active = false;
AlarmId idA[Condition][Numbers];

void setup_alarm() {
  for (int i1 = 0; i1 < Condition; i1++) {
    int thatCondition = i1;//idWidget
    String jsonCondition = LoadCondition(thatCondition);//загружаем условия кнопки 0;
    Serial.println("Setup timer" + jsonCondition);
    if (jsonCondition != "null") {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& rootjs = jsonBuffer.parseObject(jsonCondition);
      unsigned int WidjetIds = rootjs["ID"];//1//в одном файле кол-во таймеров
      unsigned int Numbers = rootjs["Numbers"];//1//в одном файле кол-во таймеров

      //for (int i=;)
      for (int i = 0; i < Numbers; i++) {//от всего колимчества таймеров

        int tID_that = rootjs["tID"][i];
        int type_that = rootjs["type"][i];
        unsigned int timer_that = rootjs["timer"][i];
        int timerType_that = rootjs["timerType"][i];
        int act_that = rootjs["act"][i];
        String actBtn_that = rootjs["actBtn"][i];
        //const char* actBtn_that_const_char = rootjs["actBtn"][i];

        int actOn_that = rootjs["actOn"][i];


        tID_a[thatCondition][i] = tID_that;
        type_a[thatCondition][i] = type_that;
        timer_a[thatCondition][i] = timer_that;
        timerType_a[thatCondition][i] =  timerType_that;
        act_a[thatCondition][i] = act_that;
        actBtn_a[thatCondition][i] = actBtn_that;
        actOn_a[thatCondition][i] = actOn_that;
        //strncpy (actBtn_a_ch[thatCondition][i], actBtn_that_const_char, sizeof(actBtn_a_ch[thatCondition][i]) - 1); //delete timerType_a;
       //actBtn_a_ch[thatCondition][i][sizeof(actBtn_a_ch[thatCondition][i]) - 1] = '\0';
       //delete actBtn_that_const_char;
        /*
          tID_a[thatCondition][i] = rootjs["tID"][i];
          type_a[thatCondition][i] = rootjs["type"][i];
          timer_a[thatCondition][i]  = rootjs["timer"][i];
          timerType_a[thatCondition][i] = ( rootjs["timerType"][i]).as<String>;
          act_a[thatCondition][i] = rootjs["act"][i];
          actBtn_a[thatCondition][i]  = ( rootjs["actBtn"][i]).as<String>;
          actOn_a[thatCondition][i]  = (rootjs["actOn"][i]).as<String>;
        */
      }
      NumberIDs[thatCondition] = Numbers;//количество в этом условии (на этой кнопке)
    }
  }
  //const char tID_ch = rootjs["tID"][0];
  //strcpy (tID_a[Numbers], tID_ch, sizeof(tID_a[Numbers])-1);

  /*
    tID_a[Numbers] = rootjs["tID"];
    type_a[Numbers] = rootjs["type"];
    timer_a[Numbers]  = rootjs["timer"];
    timerType_a[Numbers]  = rootjs["timerType"];//надо переделать в int
    act_a[Numbers] = rootjs["act"];
    actBtn_a[Numbers]  = rootjs["actBtn"];
    actOn_a[Numbers]  = rootjs["actOn"];//надо переделать в int
  */

  /*

    {
    "tID": [
    0,
    1
    ],
    "type": [
    3,
    3
    ],
    "timer": [
    "10",
    "20"
    ],
    "timerType": [
    0,
    0
    ],
    "act": [
    2,
    2
    ],
    "actBtn": [
    "cooler",
    "light"
    ],
    "actOn": [
    2,
    2
    ],
    "ID": null,
    "Numbers": 2
    }

  */
  //  setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger at specific times
  //  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  //  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day
  //  Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  //  Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  //  id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds
  //  Alarm.timerOnce(10, OnceOnly);            // called once after 10 seconds

}
void check_if_there_timer_once(String Topic, int idWidget, int NewValue) {

  Serial.println("Chek for Timer id:" + String(idWidget) + " NumberIds:" + String(NumberIDs[idWidget]));

  for (int i = 0; i < NumberIDs[idWidget]; i++) {
    Serial.println("Check if the timer");
    //Serial.println(type_a[idWidget][i]);

    if (type_a[idWidget][i] == 3) {//таймер
      //alarm_is_active = true;
      //Serial.println("Setup timer:" + String( timer_a[idWidget][i]) + " sec");
      idA[idWidget][i] = Alarm.timerOnce( timer_a[idWidget][i], OnceOnly);
      Serial.println("Setup timer:" + String( timer_a[idWidget][i]) + " sec" + " idAlarm:" + String(idA[idWidget][i]));
    }
  }
}
void loop_alarm() {

  //if (alarm_is_active) {
  Alarm.delay(0); // wait one second between clock display
  //}

}

// functions to be called when an alarm triggers:
void MorningAlarm() {
  Serial.println("Alarm: - turn lights off");
}

void EveningAlarm() {
  Serial.println("Alarm: - turn lights on");
}

void WeeklyAlarm() {
  Serial.println("Alarm: - its Monday Morning");
}

void ExplicitAlarm() {
  Serial.println("Alarm: - this triggers only at the given date and time");
}

void Repeats() {
  Serial.println("15 second timer");
}

void Repeats2() {
  Serial.println("2 second timer");
}

void OnceOnly() {
  AlarmId idAlarm = Alarm.getTriggeredAlarmId();
  Serial.println ("Alarm Triggered. ID:" + String(idAlarm));
  int that_condtion_widget = -1;
  int that_number_cond = -1;
  for (int i1 = 0; i1 < Condition; i1++) {
    int Numbers_that = NumberIDs[i1];
    for (int i = 0; i < Numbers_that; i++) {//о
      if (idA[i1][i] == idAlarm) {
        that_condtion_widget = i1;
        that_number_cond = i;
        break; break;
      }
    }
  }
  if ((that_condtion_widget != -1) && (that_number_cond != -1)) {
    Serial.println("that_condition:" + String(that_condtion_widget) + " that_number_cond:" + String(that_number_cond));
    int i = that_condtion_widget;
    int payload_is = actOn_a[that_condtion_widget][that_number_cond];
    int id_button = -1;
    for (int i2 = 0; i2 < nWidgets; i2++) {
      if (descr[i2] == actBtn_a[that_condtion_widget][that_number_cond]) {
        id_button = i2;
        //Serial.println(actBtn_a_ch[that_condtion_widget][that_number_cond]);
        break;
      }
    }

    if (act_a[that_condtion_widget][that_number_cond] == 2) { //"нажать кнопку"


      if (id_button != -1) {
        if (payload_is == 2) {//выкл
          newValue = 0; // inverted
          stat[id_button] = stat0;
        } else if (payload_is == 1) { //вкл
          newValue = 1;
          stat[id_button] = stat1;
        }
        else if (payload_is == 3) { //шим
          //pwm value
        }
        //для вкл или выкл
        digitalWrite(pin[id_button], newValue);
        pubStatusWS(sTopic[id_button], stat[id_button], true);
      }
    }
    else if (act_a[that_condtion_widget][that_number_cond] == 4) { //"отправить Email"
      String buffer;
      buffer = "сработала тревога на датчике:" + String(descr[that_condtion_widget]) + " топик:" + sTopic[that_condtion_widget] + " на пине:" + String(digitalRead(pin[that_condtion_widget]));
      sendEmail(buffer);
    }
  }
  else {
    Serial.println("Timer ERROR id:" + String(that_condtion_widget) + " numberCondtioins:" + String(that_number_cond));

  }
  Serial.println("TimerID :" + String(idAlarm) + " tIDa" + String(tID_a[that_condtion_widget][that_number_cond]) + " type_a:" + String(type_a[that_condtion_widget][that_number_cond]) + " actBtn_a:" + String(actBtn_a[that_condtion_widget][that_number_cond]));
  //alarm_is_active = false;
  // use Alarm.free() to disable a timer and recycle its memory.
  // Alarm.free(alalrmId);
  // optional, but safest to "forget" the ID after memory recycled
  //alalrmId = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}

