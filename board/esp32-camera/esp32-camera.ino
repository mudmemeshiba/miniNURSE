#include <WiFi.h>
#include <PubSubClient.h>
#include <TridentTD_LineNotify.h>

#include "esp_camera.h"
#include "esp_system.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "config.h"

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void connect_mqtt() {
  printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connect("", MQTT_USER, MQTT_PASS)) {
    printf("Failed to connect to MQTT broker.\n");
    for (;;) {}
  }
  mqtt.setCallback(mqtt_callback);
  mqtt.subscribe(TOPIC_capture);
  printf("MQTT broker connected.\n");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_capture) == 0) {
    payload[length] = 0;
    if (atoi((char*)payload) == 1){
      Camera_capture();
    }
  }
}

void setup() {

  Serial.begin(115200);
   while (!Serial) {  ;  }
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("WiFi connecting to %s\n",  WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    fflush(stdout);
    delay(500);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  
  LINE.setToken(LINE_TOKEN);

  connect_mqtt();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_SXGA; 
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  s->set_gain_ctrl(s, 1);                
  s->set_exposure_ctrl(s, 1);
  s->set_awb_gain(s, 1);
  s->set_brightness(s, 1);
  Serial.printf("init sucess \n");
}

void loop() {
    mqtt.loop();
}

void Camera_capture() {
  Serial.println("Enter CamCap");
  camera_fb_t * fb = esp_camera_fb_get(); 
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }
   Serial.println("Send to Line");
   esp_camera_fb_return(fb); 
   Send_line(fb->buf,fb->len);
}

void Send_line(uint8_t *image_data,size_t image_size){
    LINE.notifyPicture("ผลการวัดความดัน",image_data, image_size);
  }