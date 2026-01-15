#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include "prototype.h"
#include "firebase.h"

// 3. Global Objects
WiFiSSLClient client; // Using the native SSL client for R4
int counter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(statusLed,OUTPUT);
  pinMode(forceStop,INPUT_PULLUP);
  delay(2000);

  // --- Connect to WiFi ---
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int retries = 0;

  while (WiFi.status() != WL_CONNECTED ) {
    digitalWrite(statusLed,HIGH);
    delay(200);
    digitalWrite(statusLed,LOW);
    delay(200);
    Serial.print(".");
    retries++; 

    if(retries >40){
      Serial.println("\nConnection time out");
      break;
    }

  }
  
  // R4 Fix: Wait for valid IP
while (WiFi.localIP() == IPAddress(0, 0, 0, 0) && WiFi.status() == WL_CONNECTED) {
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(statusLed, HIGH); // FIXED: Changed ledPin to statusLed
  }
}

void loop() { 
  int buttonState = digitalRead(forceStop);
  if(buttonState == HIGH){
   counter++;
  
  Serial.print("Sending counter value: ");
  Serial.println(counter);
  sendToFirebase(counter);
    delay(3000); // Send every 3 seconds
  }
  
  else{
    Serial.println("Pause button press. release to continue");
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    delay(500);
  }
}


/*void setup() {
  pinMode(13, OUTPUT); // Or pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH); // Turn the LED on
  delay(1000);                     // Wait for a second
  digitalWrite(13, LOW);  // Turn the LED off
  delay(1000);                     // Wait for a second
}*/
