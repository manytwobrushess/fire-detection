#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include "network_config.h"
#include "firebase.h"
#include "sensors.h"
#include "timee.h"

// Global Objects
WiFiSSLClient client; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800);
int counter = 0;

void setup() {
  Serial.begin(115200);
  
  initSensors(); 
  
  pinMode(statusLed, OUTPUT);
  pinMode(forceStop, INPUT_PULLUP);
  delay(2000);

  // --- Connect to WiFi ---
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int retries = 0;

  while (WiFi.status() != WL_CONNECTED ) {
    digitalWrite(statusLed, HIGH);
    delay(200);
    digitalWrite(statusLed, LOW);
    delay(200);
    Serial.print(".");
    retries++; 

    if(retries > 40){
      Serial.println("\nConnection time out");
      break;
    }
  }
  
  // Wait for IP
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
    digitalWrite(statusLed, HIGH); 
  }
    timeClient.begin();
}

void loop() { 
  timeClient.update();
  String timeString = getFullFormattedTime(timeClient.getEpochTime());

  float temp = readTemperatureC();
  bool flame = flameDetected();
  int gas = readGas();

  String condition = "SAFE"; 

  // --- PRIORITY LOGIC ---
  if (flame) {
    condition = "DANGER";
    setRGB(true, false, false); 
    buzzer_on(2200);
    lcdTemp("DANGER", temp);
    delay(500); 
  }
  else if (gas > MQ2_THRESHOLD) {
    condition = "ALERT";
    setRGB(false, false, true);
    buzzer_on(1500);
    lcdGas("ALERT", gas);
    delay(500);
  }
  else if (temp > TEMP_THRESHOLD_C) {
    condition = "WARNING";
    setRGB(true, false, false);
    buzzer_off();
    lcdTemp("WARNING", temp);
  }
  else {
    condition = "SAFE";
    setRGB(false, true, false);
    buzzer_off();
    lcdTemp("SAFE", temp);
  }


  // --- SENDING LOGIC ---
  int buttonState = digitalRead(forceStop);
  if(buttonState == HIGH){
    counter++;

    Serial.println("\n-----------------------------");
    Serial.print("Status: "); 
    Serial.print(condition);
    Serial.print(" | Temp: "); 
    Serial.print(temp, 1); 
    Serial.print("C");
    Serial.print(" | Gas: "); 
    Serial.print(gas);
    Serial.print(" | Flame: "); 
    Serial.println(flame ? "YES" : "NO");  

    Serial.print("Sending NEW data: ");
    Serial.println(counter);
    

    sendToFirebase(counter, condition, temp, gas, flame, timeString);
    
    delay(3000); 
  }
  else{
    Serial.println("Pause button press. release to continue");
    digitalWrite(statusLed, LOW);
    delay(500);
    digitalWrite(statusLed, HIGH);
    delay(500);
  }
}