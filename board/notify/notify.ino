#include "config.h"
#include "time.h"
#include "buzzer.h"
#include "parseJson.h"

#include <stdlib.h>
#include <WiFi.h> 
#include <Wire.h>
#include <WiFiUdp.h>
#include <TimeLib.h>  
#include <NTPClient.h>                              
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>

/* OLED */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClient wifiClient;
WiFiUDP ntpUDP;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);
int jsonIndex;
int inTopic = 0;

/* ntpCLOCK variables */
constexpr const char* ntpServer = "asia.pool.ntp.org";
constexpr long gmtOffset_sec = 25200;
constexpr int daylightOffset_sec = 0;

byte hours_;
byte minutes_;
byte seconds_;
byte days_;
byte months_;
byte years_;
/* ntpCLOCK variables */

const int numTones = sizeof(notes) / sizeof(notes[0]);
int getFrequency(const char* noteName) {
  for (int i = 0; i < numTones; ++i) {
    if (strcmp(noteName, notes[i]) == 0) {
      return frequencies[i];
    }
  }
  return -1;
}

void update_time()
{
  char dateStr[20];
  char timeStr[9];
  char dateTime[25];

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  hours_   = timeinfo.tm_hour;
  minutes_ = timeinfo.tm_min;
  seconds_ = timeinfo.tm_sec;
  days_    = timeinfo.tm_mday;
  months_  = timeinfo.tm_mon;
  years_   = timeinfo.tm_year;

  sprintf(dateStr, "%02d/%02d/%d", days_, months_ + 1, 1900 + years_);
  sprintf(timeStr, "%02d:%02d:%02d", hours_, minutes_, seconds_);

  display.fillRect(0, 0, 128, 10, SSD1306_BLACK);
  if (inTopic == 0) {
    display.println("---------------------");
    display.println(" !! Stay hydrated !! ");
    display.println(" ");
    display.println("     O- > _ < -O     ");
    display.println(" ");
    display.println("---------------------");
    display.println(" ");
  }
  display.setCursor(0, 0);
  display.println(dateStr);
  display.setCursor(70, 0);
  display.println(timeStr);

  display.display(); 

  Serial.println(timeStr);

  strcpy(dateTime, dateStr);
  strcat(dateTime, " ");
  strcat(dateTime, timeStr); 
  mqtt.publish(TOPIC_NTPTIME, timeStr);
  delay(1000);
}

void connect_wifi() {
  printf("WiFi MAC address is %s\n", WiFi.macAddress().c_str());
  printf("Connecting to WiFi %s.\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    printf(".");
    fflush(stdout);
    delay(500);
  }
  printf("\nWiFi connected.\n");
}

void connect_mqtt() {
  printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connect("", MQTT_USER, MQTT_PASS)) {
    printf("Failed to connect to MQTT broker.\n");
    for (;;) {}
  }
  mqtt.setCallback(mqtt_callback);
  mqtt.subscribe(TOPIC_TABLE_ONE);
  mqtt.subscribe(TOPIC_TABLE_TWO);
  mqtt.subscribe(TOPIC_NTPTIME);
  printf("MQTT broker connected.\n");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  /**************************** node red table 1 ****************************/
  Serial.println("in callback");
  inTopic = 0;
  if (strcmp(topic, TOPIC_TABLE_ONE) == 0) {
    Serial.println("in topic 1");
    int table = 1;
    inTopic = 1;
    
    display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
    if (length > (MAX_JSON_LENGTH - jsonIndex - 1)) {
      jsonIndex = 0;
      return;
    }
  
    memcpy(jsonBuffer + jsonIndex, payload, length);
    jsonIndex += length;
    jsonBuffer[jsonIndex] = '\0';

    Serial.println("before getjson");
    getLineJSON(table);
    jsonIndex = 0;

    /** MAIN LOOP **/
    printf("%d %d\n",hours_,minutes_);
    while (!((hours_ == atoi(user_hours)) && (minutes_ == atoi(user_minutes)))) {
      update_time();
      delay(1000);
    }
    printf("it's time \n");

    ldr = 100 - analogRead(LDR1_GPIO)/40.95;
    printf("%d \n before buzzer", ldr);

    bool buzzerSounding = true;
    while (ldr < 60 && buzzerSounding) {
      for (int i = 0; i < sizeof(scoreIdol) / sizeof(scoreIdol[0]); ++i) {
        if (strcmp(scoreIdol[i], "Rest") == 0) {
          delay(durationIdol[i]); 
          if (ldr >= 60) {
            printf("stopBuzzer\n");
            noTone(BUZZER);
            buzzerSounding = false;
            break;
          } else {
            tone(BUZZER, getFrequency(scoreIdol[i]));
            ldr = 100 - analogRead(LDR1_GPIO)/40.95;
            printf("%d \n inside buzzer", ldr);
            if (ldr >= 60) {
              printf("stopBuzzer\n");
              noTone(BUZZER);
              buzzerSounding = false;
              break;
            }
          }
          delay(durationIdol[i]);
        }
      }
    }
  }
  /**************************** node red table 1 ****************************/
  
  /**************************** node red table 2 ****************************/
  if (strcmp(topic, TOPIC_TABLE_TWO) == 0) {
    Serial.println("in topic 2");
    int table = 2;
    inTopic = 1;

    display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
    if (length > (MAX_JSON_LENGTH - jsonIndex - 1)) {
      jsonIndex = 0;
      return;
    }
  
    memcpy(jsonBuffer + jsonIndex, payload, length);
    jsonIndex += length;
    jsonBuffer[jsonIndex] = '\0';

    Serial.println("before getjson");
    getLineJSON(table);
    jsonIndex = 0;

    /** MAIN LOOP **/
    printf("%d %d\n",hours_,minutes_);
    while (!((hours_ == atoi(user_hours)) && (minutes_ == atoi(user_minutes)))) {
      update_time();
      // display
      delay(1000);
    }
    printf("it's time for exercise \n");

    for (int i = 0; i < sizeof(scoreDoAhDeer) / sizeof(durationDoAhDeer[0]); ++i) {
      tone(BUZZER, getFrequency(scoreDoAhDeer[i]));
      delay(durationDoAhDeer[i]);
    }
    noTone(BUZZER);
  }
  /**************************** node red table 2 ****************************/
}

void setup() {
  Serial.begin(115200);
  Wire.begin(48, 47);
  pinMode(BUZZER, OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    printf("SSD1306 allocation failed");
    for(;;);
  }

  display.clearDisplay();
  display.fillRect(0, 0, 128, 64, SSD1306_BLACK);
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("---------------------");
  display.println(" miniNURSE is ready! ");
  display.println(" ");
  display.println("     <- ^ _ ^ ->     ");
  display.println(" ");
  display.println("---------------------");
  display.println(" ");
  display.display();
  delay(5000);

  connect_wifi();
  connect_mqtt();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  display.clearDisplay();
  update_time();
  Serial.println("Initialization done! \n");
}

void loop() {
  display.clearDisplay();
  mqtt.loop();
  update_time();
}
