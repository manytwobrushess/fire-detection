#include "timee.h"

String getFullFormattedTime(unsigned long epochTime) {
  unsigned long rawTime = epochTime;
  
  // Calculate Time
  unsigned long currentSecond = rawTime % 60;
  rawTime /= 60;
  unsigned long currentMinute = rawTime % 60;
  rawTime /= 60;
  unsigned long currentHour = rawTime % 24;
  unsigned long days = rawTime / 24; 

  // Calculate Year
  int currentYear = 1970;
  while (true) {
    bool isLeap = (currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0);
    int daysInYear = isLeap ? 366 : 365;
    if (days >= daysInYear) {
      days -= daysInYear;
      currentYear++;
    } else {
      break;
    }
  }

  // Calculate Month
  int currentMonth = 1;
  bool isLeap = (currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0);
  int daysInMonth[] = {31, (isLeap ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  for (int i = 0; i < 12; i++) {
    if (days >= daysInMonth[i]) {
      days -= daysInMonth[i];
      currentMonth++;
    } else {
      break;
    }
  }
  
  int currentDay = days + 1; 

  // Format with leading zeros
  String monthStr = (currentMonth < 10) ? "0" + String(currentMonth) : String(currentMonth);
  String dayStr = (currentDay < 10) ? "0" + String(currentDay) : String(currentDay);
  String hourStr = (currentHour < 10) ? "0" + String(currentHour) : String(currentHour);
  String minStr = (currentMinute < 10) ? "0" + String(currentMinute) : String(currentMinute);
  String secStr = (currentSecond < 10) ? "0" + String(currentSecond) : String(currentSecond);

  // Return: "2026-01-17_18-30-05"
  return String(currentYear) + "-" + monthStr + "-" + dayStr + "_" + hourStr + "-" + minStr + "-" + secStr;
}