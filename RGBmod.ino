#include <ArduinoJson.h>

int red = 255;
int blue = 255; 
int green = 255;

void setup() {
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
}

void loop() {
  if(Serial.available() > 0){ 
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& rootIN = jsonBuffer.parseObject(Serial);
    if(rootIN.get<int>("ID") == 2){   
      if(rootIN.get<int>("Command") == 1){
        red = rootIN.get<int>("red");  
        green = rootIN.get<int>("green");  
        blue =rootIN.get<int>("blue");  
      }
    }
    else delay(200);
  }
  
  analogWrite(5, red);
  analogWrite(3, green);
  analogWrite(6, blue);

}
