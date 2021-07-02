long previousMillis[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 



void pulse_loop(int port,int cycle,int cycle_lenght){
unsigned long currentMillis;
currentMillis= millis();
  if(currentMillis - previousMillis[port] > cycle) { digitalWrite(pin[port], LOW); }
  else{digitalWrite(pin[port], HIGH); }
  if(currentMillis - previousMillis[port] > cycle_lenght) {  previousMillis[port] = currentMillis;  }  
}
