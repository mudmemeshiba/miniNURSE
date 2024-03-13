#include "config.h"
#include "libraries.h"
#include "time.h"

/* OLED */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClient wifiClient;
WiFiUDP ntpUDP;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000); // connect to UTC+7 (Asia/Bangkok)

/* ntpCLOCK variables */
const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

byte hours_;
byte minutes_;
byte seconds_;
byte days_;
byte months_;
byte years_;

char timeStr[] = "00:00:00";
char dateStr[] = "00/00/00";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
/* ntpCLOCK variables */

/* Receiving and parsing data */
char jsonBuffer[MAX_JSON_LENGTH];
int jsonIndex = 0;
int ldr;

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

char user_hours[4];
char user_minutes[4];

const char* notes[] = {
  "B0", "C1", "CS1", "D1", "DS1", "E1", "F1", "FS1",
  "G1", "GS1", "A1", "AS1", "B1", "C2", "CS2", "D2",
  "DS2", "E2", "F2", "FS2", "G2", "GS2", "A2", "AS2",
  "B2", "C3", "CS3", "D3", "DS3", "E3", "F3", "FS3",
  "G3", "GS3", "A3", "AS3", "B3", "C4", "CS4", "D4",
  "DS4", "E4", "F4", "FS4", "G4", "GS4", "A4", "AS4",
  "B4", "C5", "CS5", "D5", "DS5", "E5", "F5", "FS5",
  "G5", "GS5", "A5", "AS5", "B5", "C6", "CS6", "D6",
  "DS6", "E6", "F6", "FS6", "G6", "GS6", "A6", "AS6",
  "B6", "C7", "CS7", "D7", "DS7", "E7", "F7", "FS7",
  "G7", "GS7", "A7", "AS7", "B7", "C8", "CS8", "D8", "DS8"
};

const int frequencies[] = {
  31, 33, 35, 37, 39, 41, 44, 46,
  49, 52, 55, 58, 62, 65, 69, 73,
  78, 82, 87, 93, 98, 104, 110, 117,
  123, 131, 139, 147, 156, 165, 175, 185,
  196, 208, 220, 233, 247, 262, 277, 294,
  311, 330, 349, 370, 392, 415, 440, 466,
  494, 523, 554, 587, 622, 659, 698, 740,
  784, 831, 880, 932, 988, 1047, 1109, 1175,
  1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865,
  1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960,
  3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978
};


const char* scoresOdeToJoy[] = {
  "E4", "E4", "F4", "G4", "G4", "F4", "E4", "D4", "C4", "C4", "D4", "E4", "E4", "D4", "D4",
  "E4", "E4", "F4", "G4", "G4", "F4", "E4", "D4", "C4", "C4", "D4", "E4", "D4", "C4", "C4"
};

const int durationOdeToJoy[] = {
  500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 750, 250, 1000,
  500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 750, 250, 1000,
};

const char* scoresIdol[] {
  "E4", "G4", "A4", "E5", "A4", "G4", "A4", "E5", "A4", "G4", "A4",
  "E4", "G4", "A4", "C5", "B4", "G4", "A4", "B4", "C5", "D5", "F5", "E5",
  "A4", "E5", "A4", "G4", "A4", "E5", "A4", "G4", "A4", "E4", "G4", "A4", "C5", "B4", "G4", "A4", "Rest", "A4", "G5", "A4"
};

const int durationIdol[] {
  250, 250, 500, 500, 500, 250, 500, 250, 250, 250, 500,
  250, 250, 500, 500, 500, 250, 500, 250, 250, 250, 500, 500,
  500, 500, 500, 250, 500, 250, 250, 250, 500, 250, 250, 500, 500, 500, 250, 250, 250, 500, 250, 500
};

const int numTones = sizeof(notes) / sizeof(notes[0]);
int getFrequency(const char* noteName) {
  for (int i = 0; i < numTones; ++i) {
    if (strcmp(noteName, notes[i]) == 0) {
      return frequencies[i];
    }
  }
  return -1;
}

COROUTINE(ntpClock) {
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();

  COROUTINE_BEGIN();
  while (true) {
    second_ = second(unix_epoch);
    if (last_second != second_) {
      last_second = second_;

      minute_ = minute(unix_epoch);
      hour_   = hour(unix_epoch);
      day_    = day(unix_epoch);
      month_  = month(unix_epoch);
      year_   = year(unix_epoch);
  
      timeStr[7]  = second_ % 10 + 48;
      timeStr[6]  = second_ / 10 + 48;
      timeStr[4]  = minute_ % 10 + 48;
      timeStr[3]  = minute_ / 10 + 48;
      timeStr[1]  = hour_ % 10 + 48;
      timeStr[0]  = hour_ / 10 + 48;
  
      dateStr[0]  = day_ / 10 + 48;
      dateStr[1]  = day_ % 10 + 48;
      dateStr[3]  = month_ / 10 + 48;
      dateStr[4]  = month_ % 10 + 48;
      dateStr[6] = (year_ / 10) % 10 + 48;
      dateStr[7] = year_ % 10 % 10 + 48;

      display.fillRect(0, 0, 128, 10, SSD1306_BLACK);
      display.setCursor(0, 0);
      display.println(dateStr);
      display.setCursor(70, 0);
      display.println(timeStr);
      display.display(); 
      mqtt.publish(TOPIC_NTPTIME, timeStr);
      Serial.println(timeStr);
    }   
    COROUTINE_YIELD();
  }
}

void update_time()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  hours_ = timeinfo.tm_hour;
  minutes_ = timeinfo.tm_min;
  seconds_ = timeinfo.tm_sec;
  // days_ = timeinfo.tm_mday;
  delay(2000);
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

void parseMedJSON(struct parsedMed& medData, const String& jsonString, int table) {
  StaticJsonDocument<400> tableOne; 
  deserializeJson(tableOne, jsonString);
  strlcpy(user_hours, tableOne["hour_med"] | "default", sizeof(user_hours));
  strlcpy(user_minutes, tableOne["minute_med"] | "default", sizeof(user_minutes));
  // DeserializationError error = deserializeJson(tableOne, jsonString);
  // if (error) {
  //   Serial.print("JSON parsing failed: ");
  //   Serial.println(error.c_str());
  //   }
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
    printf("This is the line from mqtt\n"); Serial.println(line);
    // line = strtok(NULL, "\n"); // DEBUG
    String lineString = String(line); // each line from file
    struct parsedMed medData;
    struct parsedActivity activityData;
    // line = strtok(NULL, "\n"); // DEBUG
    
    /**************************** parsing table 1 ****************************/
    if (table == 1) {
      parseMedJSON(medData, lineString.c_str(), table); // PARSING JSON

      // display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
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
      line = strtok(NULL, "\n"); // DEBUG: moved
    }
    /**************************** parsing table 1 ****************************/

    /**************************** parsing table 2 ****************************/
    if (table == 2) {
      parseActivityJSON(activityData, lineString.c_str(), table);

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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  /**************************** node red table 1 ****************************/
  Serial.println("in callback");
  if (strcmp(topic, TOPIC_TABLE_ONE) == 0) {
    Serial.println("in topic 1");
    int table = 1;
    
    display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
    if (length > (MAX_JSON_LENGTH - jsonIndex - 1)) {
      jsonIndex = 0;
      return;
    }
  
    memcpy(jsonBuffer + jsonIndex, payload, length);
    jsonIndex += length;
    jsonBuffer[jsonIndex] = '\0'; // Null-terminate the buffer

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
    printf("it's time \n");

    ldr = 100 - analogRead(LDR1_GPIO)/40.95;
    printf("%d \n before buzzer", ldr);

    bool buzzerSounding = true;
    while (ldr < 60 && buzzerSounding) {
      for (int i = 0; i < sizeof(scoresIdol) / sizeof(scoresIdol[0]); ++i) {
        if (strcmp(scoresIdol[i], "Rest") == 0) {
          delay(durationIdol[i]); 
          if (ldr >= 60) {
            printf("stopBuzzer\n");
            noTone(BUZZER);
            buzzerSounding = false;
            break;
          }
        } else {
          tone(BUZZER, getFrequency(scoresIdol[i]));
          ldr = 100 - analogRead(LDR1_GPIO)/40.95;
          printf("%d \n inside buzzer", ldr);
          if (ldr >= 60) {
            printf("stopBuzzer\n");
            noTone(BUZZER);
            buzzerSounding = false;
            break;
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

    display.fillRect(0, 10, 128, 54, SSD1306_BLACK);
    if (length > (MAX_JSON_LENGTH - jsonIndex - 1)) {
      jsonIndex = 0;
      return;
    }
  
    memcpy(jsonBuffer + jsonIndex, payload, length);
    jsonIndex += length;
    jsonBuffer[jsonIndex] = '\0'; // Null-terminate the buffer

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

    for (int i = 0; i < sizeof(scoresOdeToJoy) / sizeof(scoresOdeToJoy[0]); ++i) {
      tone(BUZZER, getFrequency(scoresOdeToJoy[i]), durationOdeToJoy[i]);
    }
  }
  /**************************** node red table 2 ****************************/
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(48, 47);
  pinMode(BUZZER, OUTPUT);
  connect_wifi();
  connect_mqtt();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    printf("SSD1306 allocation failed");
    for(;;);
  }
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  update_time();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("miniNURSE is ready!");
  display.println(" ");
  display.println("         +         ");
  display.println(" ");
  display.display();
  delay(3000);
  Serial.println("start loop \n");
}

void loop()
{
  // Serial.println("hello loop");
  display.clearDisplay();
  mqtt.loop();
  update_time();
  printf("%d : %d\n", hours_, minutes_);
  ntpClock.runCoroutine();
}