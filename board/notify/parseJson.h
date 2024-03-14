#pragma once
#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/* Receiving and parsing data */
extern char jsonBuffer[MAX_JSON_LENGTH];
extern int ldr;
extern char user_hours[4];
extern char user_minutes[4];
/* Receiving and parsing data */

/* User input */
struct parsedMed {
  char medicine[30];
  char amount[10];
  char hour_med[4];
  char minute_med[4];
  char details[50];
};

struct parsedActivity {
  char activity[30];
  char hour_activity[4];
  char minute_activity[4];
};
/* User input */

void parseMedJSON(struct parsedMed& medData, const String& jsonString, int table);
void parseActivityJSON(struct parsedActivity& activityData, const String& jsonString, int table);
void getLineJSON(int table);