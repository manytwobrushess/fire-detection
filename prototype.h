#ifndef PROTOTYPE_H
#define PROTOTYPE_H
// Replace these with your network info
const char ssid[] = "iPhone";
const char pass[] = "yxistiancai";

// Replace with your Firebase project info
#define API_KEY "kyuGcuZ3iki4t7J7AlnxLER3Nu5lfHsbt7oSS6UW"
#define DATABASE_HOST "ffiredetection-1673c-default-rtdb.asia-southeast1.firebasedatabase.app"
#define statusLed 13
#define forceStop 7

void sendToFirebase(int value);

#endif