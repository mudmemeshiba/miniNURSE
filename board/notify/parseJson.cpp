#include "parseJson.h"
#include "config.h"

char jsonBuffer[MAX_JSON_LENGTH];
int ldr = 0; 
char user_hours[4];
char user_minutes[4];

void parseMedJSON(struct parsedMed& medData, const String& jsonString, int table) {
  StaticJsonDocument<400> tableOne; 
  deserializeJson(tableOne, jsonString);
  strlcpy(user_hours, tableOne["hour_med"] | "default", sizeof(user_hours));
  strlcpy(user_minutes, tableOne["minute_med"] | "default", sizeof(user_minutes));

  strcpy(medData.medicine, tableOne["medicine"]);
  strcpy(medData.amount, tableOne["amount"]);
  strcpy(medData.hour_med, tableOne["hour_med"]);
  strcpy(medData.minute_med, tableOne["minute_med"]);
  strcpy(medData.details, tableOne["details"]);
}

void parseActivityJSON(struct parsedActivity& activityData, const String& jsonString, int table) {
  StaticJsonDocument<400> tableTwo; 
  deserializeJson(tableTwo, jsonString);
  strlcpy(user_hours, tableTwo["hour_activity"] | "default", sizeof(user_hours));
  strlcpy(user_minutes, tableTwo["minute_activity"] | "default", sizeof(user_minutes));

  strcpy(activityData.activity, tableTwo["activity"]);
  strcpy(activityData.hour_activity, tableTwo["hour_activity"]);
  strcpy(activityData.minute_activity, tableTwo["minute_activity"]);
}

void getLineJSON(int table) {
  char* line = strtok(jsonBuffer, "\n");
  while (line != NULL) {
    Serial.println("This is the line from mqtt "); Serial.println(line);
    String lineString(line); 
    struct parsedMed medData;
    struct parsedActivity activityData;
    
    /**************************** parsing table 1 ****************************/
    if (table == 1) {
      parseMedJSON(medData, lineString.c_str(), table);

      display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
      display.setCursor(0, 15);
      display.print("Med: "); display.println(medData.medicine);
      Serial.println(medData.medicine);
      display.print("Amount: "); display.println(medData.amount);
      Serial.println(medData.amount);
      display.print("Time: "); display.print(medData.hour_med);
      Serial.println(medData.hour_med);
      display.print(":"); display.println(medData.minute_med);
      Serial.println(medData.minute_med);
      display.print("Notes: "); display.println(medData.details);
      Serial.println(medData.details);
      display.display();
      delay(10);
      line = strtok(NULL, "\n"); 
    }
    /**************************** parsing table 1 ****************************/

    /**************************** parsing table 2 ****************************/
    if (table == 2) {
      parseActivityJSON(activityData, lineString.c_str(), table);

      display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
      display.setCursor(0, 15);
      display.print("Activity: "); display.println(activityData.activity);
      Serial.println(activityData.activity);
      display.print("Time: "); display.print(activityData.hour_activity);
      Serial.println(activityData.hour_activity);
      display.print(":"); display.println(activityData.minute_activity);
      Serial.println(activityData.minute_activity);
      display.display();
      delay(10);
      line = strtok(NULL, "\n");
    }
    /**************************** parsing table 2 ****************************/
  }
}