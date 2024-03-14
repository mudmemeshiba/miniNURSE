#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIFI_SSID    "HomeMMM_2.4G"
#define WIFI_PASS    "63666366"

#define LINE_TOKEN   "WcwZfC55AeE8PXBsn6ClOx2TKN7abDW8BNDerztMvEs"
#define MQTT_BROKER  "iot.cpe.ku.ac.th"
#define MQTT_USER    "b6610502153"  
#define MQTT_PASS    "pacharamon.put@ku.th"      
#define TOPIC_PREFIX "b6610502153"  

#define TOPIC_TABLE_ONE    TOPIC_PREFIX "/table1"  // from FirstBox
#define TOPIC_TABLE_TWO    TOPIC_PREFIX "/table2"  // from SecondBox
#define TOPIC_NTPTIME      TOPIC_PREFIX "/ntpTime" // from esp32
#define TOPIC_SONGCHOICE   TOPIC_PREFIX "/songchoice"

#define BUZZER          15
#define LDR1_GPIO       4
#define SW_PIN          2

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define MAX_JSON_LENGTH 400

extern Adafruit_SSD1306 display;