#define WIFI_SSID    "YOUR_WIFI_SSID"
#define WIFI_PASS    "YOUR_WIFI_PASSWORD"

#define LINE_TOKEN   "WcwZfC55AeE8PXBsn6ClOx2TKN7abDW8BNDerztMvEs"
#define MQTT_BROKER  "iot.cpe.ku.ac.th"
#define MQTT_USER    "b6610502153"  
#define MQTT_PASS    "pacharamon.put@ku.th"      
#define TOPIC_PREFIX "b6610502153"  

// #define TOPIC_LED_RED      TOPIC_PREFIX "/led/red"
// #define TOPIC_LED_GREEN    TOPIC_PREFIX "/led/green"
// #define TOPIC_LED_YELLOW   TOPIC_PREFIX "/led/yellow"
// #define TOPIC_SWITCH       TOPIC_PREFIX "/switch"
// #define TOPIC_DISPLAY_TEXT TOPIC_PREFIX "/display/text"
// #define TOPIC_LIGHT        TOPIC_PREFIX "/light"

#define TOPIC_TABLE_ONE    TOPIC_PREFIX "/table1"  // from FirstBox
#define TOPIC_TABLE_TWO    TOPIC_PREFIX "/table2"  // from SecondBox
#define TOPIC_NTPTIME      TOPIC_PREFIX "/ntpTime" // from esp32

// #define RED_GPIO       42
// #define YELLOW_GPIO    41
// #define GREEN_GPIO     40

#define BUZZER         15
#define LDR1_GPIO      4
#define SW_PIN         2

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64

#define MAX_JSON_LENGTH 400 // more??