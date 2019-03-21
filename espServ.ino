#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

 
ESP8266WebServer server(80);

const char* ssid = "UPC7D78489";
const char* password =  "Nk5tjnwx4kzu";

void setup() {

    Serial.begin(115200);
    WiFi.begin(ssid, password);  //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
         delay(500);
        Serial.println("Waiting to connect...");
 
    }


    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
 
    server.on("/body", handleBody); //Associate the handler function to the path
 
    server.begin(); //Start the server
    Serial.println("Server listening");

}
 
void loop() {
    server.handleClient(); 
}

void handleBody() { //Handler for the body path
      if (server.hasArg("plain")== false){ //Check if body received
          server.send(200, "text/plain", "Body not received");
          return;
      }
       
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parse(server.arg("plain"));
    
    if(root["Command"] == "RefreshPower"){
        JsonObject& rootOUT = jsonBuffer.createObject();
        rootOUT["ID"] = 1;
        rootOUT["Command"] = 1;
        rootOUT.printTo(Serial);
    } 
    else if(root["Command"] == "DeviceOnOff"){
       JsonObject& rootOUT = jsonBuffer.createObject();
        rootOUT["ID"] = 1;
        rootOUT["Command"] = 2;
        rootOUT.printTo(Serial);
    }
    else if(root["Command"] == "SetLight"){
       JsonObject& rootOUT = jsonBuffer.createObject();
        rootOUT["ID"] = 2;
        rootOUT["Command"] = 1;
        rootOUT["red"] = root["red"];
        rootOUT["green"] = root["green"];
        rootOUT["blue"] = root["blue"];
        rootOUT.printTo(Serial);
    }
    
    if(Serial.available() > 0){ 
        JsonObject& rootIN = jsonBuffer.parse(Serial);
        String output2;
        rootIN.printTo(output2);
        if(rootIN["ID"] == 1){
           server.send(200, "text/plain", output2);
        }
        
    }

    jsonBuffer.clear();
    server.send(200, "text/plain", "");
}



 /*
void handleBody() { //Handler for the body path
 
      if (server.hasArg("plain")== false){ //Check if body received
             server.send(200, "text/plain", "Body not received");
            return;
      }
      
      String message = "Body received:\n";
             message += server.arg("plain");
             message += "\n";

      server.send(200, "text/plain", server.arg("plain"));
      Serial.println(message);
}*/
