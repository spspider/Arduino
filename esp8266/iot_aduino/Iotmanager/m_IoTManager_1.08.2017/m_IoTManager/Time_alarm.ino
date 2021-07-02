/*
   TimeAlarmExample.pde

   This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
   and simulates turning lights on at night and off in the morning
   A weekly timer is set for Saturdays at 8:30:30

   A timer is called every 15 seconds
   Another timer is called once only after 10 seconds

   At startup the time is set to Jan 1 2011  8:29 am
*/
/*
#include <TimeLib.h>
#include <TimeAlarms.h>


const unsigned int Numbers = 5;

AlarmId alalrmId;

int tID_a[Numbers];
int type_a[Numbers];
unsigned int timer_a[Numbers];
char timerType_a[Numbers][20];//надо переделать в int
int act_a[Numbers];
char actBtn_a[Numbers][20];
char actOn_a[Numbers][20];//надо переделать в int

AlarmId idA[Numbers];

void setup_alarm() {

  String jsonCondition = LoadCondition(0);//загружаем условия кнопки 0;
  Serial.println("Setup timer"+jsonCondition);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(jsonCondition);
  unsigned int NumberID = rootjs["ID"];

  //for (int i=;)
  int tID_that = rootjs["tID"][0];
  int type_that = rootjs["type"][0];
  unsigned int timer_that = rootjs["timer"][0];
  const char *timerType_that = rootjs["timerType"][0];
  int act_that = rootjs["act"][0];
  const char *actBtn_that = rootjs["actBtn"][0];
  const char *actOn_that = rootjs["actOn"][0];


  tID_a[0] = tID_that;
  type_a[0] = type_that;
  timer_a[0] = timer_that;
  strncpy (timerType_a[0], timerType_that, sizeof(timerType_a[0]) - 1);
  act_a[0] = act_that;
  strncpy (actBtn_a[0], actBtn_that, sizeof(actBtn_a[0]) - 1);
  strncpy (actOn_a[0], actOn_that, sizeof(actOn_a[0]) - 1);
  timerType_a[0][sizeof(timerType_a[0]) - 1] = '\0';
  actBtn_a[0][sizeof(actBtn_a[0]) - 1] = '\0';
  actOn_a[0][sizeof(actOn_a[0]) - 1] = '\0';

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
  if (type_that == 3) {
    idA[0] = Alarm.timerOnce(timer_that, OnceOnly);
  }

  for (int i = 0; i < Numbers; i++) {
    //if (type_a[i] == 3) {
    //int act = act_a[i];
    //String actBtn = actBtn_a[i];
    //String actOn = actOn_a[i];
    //idA[i] = Alarm.timerOnce(timer_a[i], OnceOnly);
    //}
  }
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
    "30",
    "20"
    ],
    "timerType": [
    "сек",
    "сек"
    ],
    "act": [
    0,
    1
    ],
    "actBtn": [
    "light",
    "cooler"
    ],
    "actOn": [
    "вкл",
    "вкл"
    ],
    "ID": null,
    "Numbers": 1
    }

  */
  /*
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

void loop_alarm() {
  Alarm.delay(1000); // wait one second between clock display
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
  Serial.println("Timer :" + String(idAlarm) + " tIDa" + tID_a[idAlarm] + " type_a:" + type_a[idAlarm] + " actBtn_a:" + actBtn_a[idAlarm]);
  // use Alarm.free() to disable a timer and recycle its memory.
  // Alarm.free(alalrmId);
  // optional, but safest to "forget" the ID after memory recycled
  //alalrmId = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}
*/
