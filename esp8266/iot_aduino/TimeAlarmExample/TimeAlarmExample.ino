/*
   TimeAlarmExample.pde

   This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
   and simulates turning lights on at night and off in the morning
   A weekly timer is set for Saturdays at 8:30:30

   A timer is called every 15 seconds
   Another timer is called once only after 10 seconds

   At startup the time is set to Jan 1 2011  8:29 am
*/

// Questions?  Ask them here:
// http://forum.arduino.cc/index.php?topic=66054.0

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

AlarmId id;
AlarmId idA[21];
unsigned int trig_sec[5];
int timer_all[5];
unsigned int timer2[5];

char timerType_a[5][20];//первый - номер массива, второй - количество букв в каждой перменной.надо переделать в int
char test[5][5][20];//первый - номер массива, второй - количество букв в каждой перменной.надо переделать в int

int called = 0;//глобальная переменная для того, что бы связать установочный таймер и список таймеров

void setup() {

  //ESP.eraseConfig();
  //ESP.reset();
  timer2[0] = 10;
  timer2[1] = 2;
  timer2[2] = 3;
  timer2[3] = 2;
  timer2[4] = 10;
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor

  setTime(8, 29, 0, 1, 1, 11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger at specific times
  //Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  //Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day
  //Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  //Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  //id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds

  //Alarm.timerRepeat(2, Repeats);
  //Alarm.timerOnce(10, OnceOnly);

  for (int i = 0; i < 5; i++) {
    setup_once(timer2[i], i); //нужно установить id каждому таймеру
    //id_timer[i] =
    idA[i] = Alarm.timerOnce(timer2[i], OnceOnly0);
  }

  /*
    int i=0;
    i = 0;
    idA[i] = Alarm.timerOnce(timer2[i], []()
    {
    OnceOnly0(i);
    });
    i = 1;
    idA[i] = Alarm.timerOnce(timer2[i], []()
    {
    OnceOnly0(i);
    });
  */
}
void OnceOnly0() {

  const char *timerType = "timerType";
  int timer = 30;
  strncpy (timerType_a[0], timerType, sizeof(timerType_a[0]) - 1); //delete timerType_a;
  strncpy (test[0][0], timerType, sizeof(test[0][0]) - 1); //delete timerType_a;

  timerType_a[0][sizeof(timerType_a[0]) - 1] = '\0';
  test[0][0][sizeof(test[0][0]) - 1] = '\0';
  timer_all[0] = timer;
  Serial.print("timerType:");

  Serial.println(timerType_a[0]);
  Serial.print(" timer_all[0]:");
  Serial.println(timer_all[0]);
  Serial.print(" test[0][0]");
  Serial.println(test[0][0]);
  Serial.println("that ID:" + String(Alarm.getTriggeredAlarmId()) + " triggerred:" + String(timer2[Alarm.getTriggeredAlarmId()]));

}
void loop() {
  //digitalClockDisplay();
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
  Serial.println("15 second timer repeat");
}

void Repeats2() {
  Serial.println("2 second timer");
}

void OnceOnly() {

  Serial.println("trigger sec Once");
  // use Alarm.free() to disable a timer and recycle its memory.
  //free(id);
  // optional, but safest to "forget" the ID after memory recycled
  //id = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}
void setup_once(int timer, int i) {
  Serial.println("setup:" + String(i) + " for timer:" + String(timer));
}

void OnceOnly1() {
  Serial.println("trigger sec OnceOnly1 2sec");
}
void OnceOnly2() {
  Serial.println("trigger sec OnceOnly2 4sec");
}
void OnceOnly3() {
  Serial.println("trigger sec OnceOnly3 6sec");
}
void OnceOnly4() {
  Serial.println("trigger sec OnceOnly4 8sec");
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

