#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// --- SHARED CONSTANTS (Main needs these) ---
const int MQ2_THRESHOLD = 4200;
const float TEMP_THRESHOLD_C = 45.0;

// --- FUNCTION DECLARATIONS ---
void initSensors();

// Reading Functions
float readTemperatureC();
bool flameDetected();
int readGas(); 

// Display/Control Functions
void setRGB(bool r, bool g, bool b);
void buzzer_on(uint16_t f = 2000);
void buzzer_off();
void lcdTemp(const char* cond, float tempC);
void lcdGas(const char* cond, int gas);

#endif