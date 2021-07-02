char incomingByte = 0;   // for incoming serial data
void test_setup() {
}
void test_loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    //byte byteArray[5];
    //strcpy((char *)incomingByte, "0123");
    String myString = String(char(incomingByte));
    saveConditiontoJson(0);

    //save_wifiList(myString, myString);
    //String *wifi_List = read_wifiList(myString.toInt());
    //int number = atoi((char*)incomingByte);
    //Serial.println(wifi_List[0]);
    //Serial.println(wifi_List[1]);
    // say what you got:

    Serial.println(char(incomingByte));

  }


}

