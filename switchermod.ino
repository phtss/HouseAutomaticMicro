#include <ArduinoJson.h>

boolean check = true;
unsigned long   times;
unsigned long   freq;

boolean check2 = true;
boolean check3 = true;
unsigned long  lastTime;
int poborMocy;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(7, OUTPUT);
  
  lastTime = millis();
  times = millis();
  freq = millis();
}



void getPower(){
  if(analogRead(A1) > 400){ 
    if(check2){
      check2 = false;
      poborMocy = 3600000/(millis()-lastTime);
      lastTime = millis();
     }
  }
  else check2 = true;
  return poborMocy;
}



 
 void blinkLed(){
  if(millis()-times > map(analogRead(A0), 0, 1023, 200, 5000)){
    if(millis()-freq > 1){
      freq = millis();
      if(check) {
        digitalWrite(2,HIGH);
        check = false;
      }
      else {
        digitalWrite(2,LOW);
        check = true;
        times = millis();
      }
    }
  }
}


void loop() {
  getPower();
  blinkLed();


  if(Serial.available() > 0){ 
    //String reader = Serial.readStringUntil('\n'); 
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& rootIN = jsonBuffer.parse(Serial);
    
    if(rootIN.get<int>("ID") == 1){   
      if(rootIN.get<int>("Command") == 1){
        JsonObject& root = jsonBuffer.createObject();
        root["ID"] = 1;
        root["actualPower"] = poborMocy;
        root.printTo(Serial);
      }
      else if(rootIN.get<int>("Command") == 2){
        if(check3){
            digitalWrite(7,HIGH);
            check3 = false;
          }
          else {
            digitalWrite(7,LOW);
            check3 = true;
          }
      }
      else {
        delay(200);
      }
    }
    //jsonBuffer.clear();
  }

}
