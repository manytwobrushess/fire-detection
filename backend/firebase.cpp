#include <Arduino.h>         
#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include "network_config.h"       
#include "firebase.h"        

extern WiFiSSLClient client; 

void sendToFirebase(int value, String status, float temperature, int gasValue, bool isFlame, String timestamp) {
  if (client.connect(DATABASE_HOST, 443)) {

String data = "{";
    data += "\"counter\":" + String(value) + ",";
    data += "\"status\":\"" + status + "\",";
    data += "\"temperature\":" + String(temperature) + ",";   
    data += "\"gas\":" + String(gasValue) + ",";    
    data += "\"flame\":" + String(isFlame);     
    data += "}";

    String fullPath = "/test/counter/" + timestamp + ".json?auth=" + String(API_KEY);
    //String timestamp = "2026-01-17_17-56-35"; // Example time
    //String fullPath = path + "/" + timestamp + ".json";

    client.print("PUT " + fullPath + " HTTP/1.1\r\n");
    client.print("Host: " + String(DATABASE_HOST) + "\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data stored under timestamp: " + timestamp);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break; 
    }
    client.stop();
    
  } else {
    Serial.println("Connection to Firebase failed.");
  }
}