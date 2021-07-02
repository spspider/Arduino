long previousMillis = 0; 


void pulse_loop(int port,int cycle,int cycle_lenght){
  unsigned long currentMillis = millis();
  //int cycle[port]=cycle;
  if(currentMillis - previousMillis > cycle) { values[1][port]=0; }
  else{values[1][port]=1;}
  if(currentMillis - previousMillis > cycle_lenght) {  previousMillis = currentMillis;  }  
  //else{values[3][port]=1;}
}
