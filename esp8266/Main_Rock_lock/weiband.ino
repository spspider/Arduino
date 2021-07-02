
#include <Wiegand.h>

WIEGAND wg;

void setup_wg() {
  //Serial.begin(115200);
  //   default Wiegand Pin 2 and Pin 3 see image on README.md
  //  for non UNO board, use wg.begin(pinD0, pinD1) where pinD0 and pinD1
  //  are the pins connected to D0 and D1 of wiegand reader respectively.
  wg.begin(wgD0, wgD1);
}
void CheckFileLinebyLine() {
  char readfile_ch[200];
  String readfile = readCommonFiletoJson("wg");
  readfile.toCharArray(readfile_ch, sizeof(readfile_ch) - 1);

}
bool searchinfile(char search_ch[20], String where) {
  char str[200];
  where.toCharArray(str, sizeof(ssid) - 1);
  char * pch;
  pch = strstr (str, search_ch);
  if (pch != NULL) {
    return 1;
  }
  return 0;
}

void loop_wg() {
  
  if (wg.available())
  {
    char buffer[20];
    sprintf (buffer, "%X\n", wg.getCode());
    bool searched = searchinfile(buffer, readCommonFiletoJson("wg"));

    if (record_card == 1) {
      if (searched) {
        Serial.println("founded!");
      } else {
        saveCommonFiletoJson("wg", buffer, 0);
        Serial.println("recordered!");
      }
    } else {
      if (searched) {
        open_lock();
      }
    }
    Serial.print("Wiegand DEC = ");
    Serial.print(buffer);
    //    Serial.print(", DECIMAL = ");
    //    Serial.print(wg.getCode());
    //    Serial.print(", Type W");
    //    Serial.println(wg.getWiegandType());
    if (wg.getCode() == 12740413) {
      //      String pos = getHttp("192.168.1.103:8080/call");
    }
  }
}
