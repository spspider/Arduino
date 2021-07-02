/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup_w433() {
  if (w433rcv != 255) {
    mySwitch.enableReceive(w433rcv);  // Receiver on interrupt 0 => that is pin #2
    Serial.println("W433_Enabled" + String(w433rcv, DEC));
  }
}

void loop_w433() {
    if (w433rcv != 255) {
  if (mySwitch.available()) {
    String codeIR;
    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      codeIR = String(mySwitch.getReceivedValue());
      Serial.println(codeIR);
      if (!Page_IR_opened) {
        if (check_code_IR(codeIR)) {
          open_lock();
        }
        // saveocde_to_file(codeIR);
      }
      else {
        server.send(200, "text/plain", codeIR);
      }
    }
    mySwitch.resetAvailable();
  }

    }
}
bool check_code_IR(String codeIR) {
  String irJson = readCommonFiletoJson("setup_433");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& rootjs = jsonBuffer.parseObject(irJson);
  unsigned int numberChosed = rootjs["num"];
  for (char i = 0; i < numberChosed; i++) {
    String IRCode = rootjs["code"][i];
    if (IRCode) {
      if (codeIR == IRCode) {
        return true;
      }
    }
  }
  return false;
}
/*
  void check_code_IR(String codeIR) {
  for (char i = 0; i < IRCodeId_numbers; i++) {
    if (IRCodeString[i] == codeIR) {//совпадение найдено
      for (char i1 = 0; i1 < nWidgets; i1++) {
        if (IrButtonID[i1] == i) {
          Serial.println("FIND IR:" + descr[i1] + " IrButtonID[i1]:" + String(IrButtonID[i1], DEC) + " i:" + String(i, DEC));
          stat[i1] ^= 1;
          save_stat();
          digitalWrite(pin[i1], stat[i1] );
          //stat[i1] = new_state;
          delay(300);
        }
      }
    }
  }
  }
*/
