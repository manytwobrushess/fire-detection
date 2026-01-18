#include "sensors.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

#define FLAME_PIN   2
#define MQ2_PIN     A0
#define NTC_A0_PIN  A1
#define BUZZER_PIN  8
#define LED_R       9
#define LED_G       10
#define LED_B       11

// NTC Settings
#define ADC_BITS 14
#define ADC_MAX  16383.0
#define VCC      5.0
#define BETA     3950.0
#define R0       10000.0
#define T0       298.15
#define R_FIXED  10000.0
#define TEMP_OFFSET 0.0
const bool NTC_TO_VCC = true;
const bool FLAME_ACTIVE_LOW = true;

// LCD 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// ===================== NEW INIT FUNCTION =====================
// Call this once in your main setup()!
void initSensors() {
  Wire.begin();
  analogReadResolution(ADC_BITS);

  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(NTC_A0_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(1000);
  lcd.clear();
}

//sensor logic
void setRGB(bool r, bool g, bool b) {
  digitalWrite(LED_R, r ? HIGH : LOW);
  digitalWrite(LED_G, g ? HIGH : LOW);
  digitalWrite(LED_B, b ? HIGH : LOW);
}

void buzzer_on(uint16_t f) { tone(BUZZER_PIN, f); }
void buzzer_off() { noTone(BUZZER_PIN); }

bool flameDetected() {
  int v = digitalRead(FLAME_PIN);
  return FLAME_ACTIVE_LOW ? (v == LOW) : (v == HIGH);
}

float readTemperatureC() {
  int adc = analogRead(NTC_A0_PIN);
  float v = adc * (VCC / ADC_MAX);
  v = constrain(v, 0.001, VCC - 0.001);

  float rNTC = NTC_TO_VCC ?
    R_FIXED * (v / (VCC - v)) :
    R_FIXED * ((VCC - v) / v);

  float tK = 1.0 / ((1.0 / T0) + (1.0 / BETA) * log(rNTC / R0));
  return (tK - 273.15) + TEMP_OFFSET;
}

int readGas(){
    return analogRead(MQ2_PIN);
}

void lcdTemp(const char* cond, float tempC) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cond);
  lcd.setCursor(0, 1);
  lcd.print("TEMP: ");
  lcd.print(tempC, 1);
  lcd.print("C");
}

void lcdGas(const char* cond, int gas) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cond);
  lcd.setCursor(0, 1);
  lcd.print("MQ2: ");
  lcd.print(gas);
}