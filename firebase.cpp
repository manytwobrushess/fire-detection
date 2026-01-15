#include <Arduino.h>         
#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include "network_config.h"       
#include "firebase.h"        

extern WiFiSSLClient client; 

void sendToFirebase(int value, String status, float temperature, int gasValue, bool isFlame) {
  if (client.connect(DATABASE_HOST, 443)) {

String data = "{";
    data += "\"counter\":" + String(value) + ",";
    data += "\"status\":\"" + status + "\",";
    data += "\"temperature\":" + String(temperature) + ",";   
    data += "\"gas\":" + String(gasValue) + ",";    
    data += "\"flame\":" + String(isFlame);     
    data += "}";

    String path = "/test/counter.json?auth=" + String(API_KEY);
//post wull be see the history

    client.print("PUT " + path + " HTTP/1.1\r\n");
    client.print("Host: " + String(DATABASE_HOST) + "\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data sent!");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break; 
    }
    client.stop();
    
  } else {
    Serial.println("Connection to Firebase failed.");
  }
}