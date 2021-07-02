void client_file_read(){

  //delay(1000); 
if (client_my.connected()) {
//if (client_my.available()) {
if ((request_now==0)||(request_now==1)){
    //Serial.print (lastConnectionTime+"\n");
    
  Serial.print("\n");
 // if (client_my.available()) {
     for (int i=0; i < 10; i++){ 
          Serial.print("\n");Serial.print(i);Serial.print(":");
          for (int i1=0; i1 < sizeofarray_fields; i1++){
          String b_string=fields[i1];b_string.concat(i);b_string.concat(":");
          b_string.toCharArray(b_print, 30);
          //Serial.print(b_print);Serial.print("\n");
          if(finder_fields.find(b_print)){  // до чего 
          values[i1][i] = finder_fields.getValue();//переводим все данные в массив
          Serial.print(fields[i1]);Serial.print(values[i1][i]);
        }
      }
    }
      if (finder_fields.find("wakeup=")){
      wakeup = finder_fields.getValue();
      if (wakeup==2){wakeup=0;}};

}
//if (request_now==1){//ищем телефон////////////////////////
//       if(finder_fields.getString("android","aea2118",buffer,sizeof(buffer))){
//          Serial.println(buffer);}
//}

/////выводим для логирования//////////////////////////
index=0;
Get_HTTP_request_read="data:"; 
for (int i3=0;i3<sizeofarray_fields;i3++){
  Get_HTTP_request_read.concat(fields[i3]);//номер поля
/////перевод в память EEPROM/////////////////////////////////////////
    for (int i2=0;i2<14;i2++){
    Get_HTTP_request_read.concat(values[i3][i2]);
    EEPROM.write(index, values[i3][i2]);  
    index++;
  }
}
/////////////////////////////////////////////////
    Serial.print("\n");
//    Serial.print(Get_HTTP_request_read);
    //Serial.print(values[3][3]+"\n");
    lastConnectionTime = millis();
    //client_my.stop();
    }

}

//}
