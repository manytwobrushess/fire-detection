#include <Arduino.h>         // Needed for Serial, String, etc.
#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include "prototype.h"       // Get your API_KEY and HOST
#include "firebase.h"        // Connect to the header

// --- THE MAGIC PART ---
// This tells the file: "Look for 'client' in the main file"
extern WiFiSSLClient client; 

void sendToFirebase(int value) {
  if (client.connect(DATABASE_HOST, 443)) {
    
    String data = String(value); 
    String path = "/test/counter.json?auth=" + String(API_KEY);

    client.print("PUT " + path + " HTTP/1.1\r\n");
    client.print("Host: " + String(DATABASE_HOST) + "\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data sent!");

    // Clear buffer
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break; 
    }
    client.stop();
    
  } else {
    Serial.println("Connection to Firebase failed.");
  }
}