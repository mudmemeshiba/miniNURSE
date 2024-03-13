from machine import Pin, ADC, SoftI2C
from hcsr04 import HCSR04
#import ssd1306
import time
import network
from config import (
    WIFI_SSID, WIFI_PASS,
    MQTT_BROKER, MQTT_USER, MQTT_PASS,
    TOPIC_PREFIX
)
from umqtt.simple import MQTTClient

TOPIC_Ultrasonic = f'{TOPIC_PREFIX}/Ultrasonic'

def connect_wifi():
    mac = ':'.join(f'{b:02X}' for b in wifi.config('mac'))
    print(f'WiFi MAC address is {mac}')
    wifi.active(True)
    print(f'Connecting to WiFi {WIFI_SSID}.')
    wifi.connect(WIFI_SSID, WIFI_PASS)
    while not wifi.isconnected():
        print('.', end='')
        time.sleep(0.5)
    print('\nWiFi connected.')

def connect_mqtt():
    print(f'Connecting to MQTT broker at {MQTT_BROKER}.')
    mqtt.connect()
    print('MQTT broker connected.')

def mqtt_callback(topic, payload):
    pass

############
# setup
############
sensor = HCSR04(trigger_pin=13, echo_pin=11, echo_timeout_us=10000)
wifi = network.WLAN(network.STA_IF)
mqtt = MQTTClient(client_id='',
                  server=MQTT_BROKER,
                  user=MQTT_USER,
                  password=MQTT_PASS)
connect_wifi()
connect_mqtt()

############
# loop
############
while True:
    # check for incoming subscribed topics
    mqtt.check_msg()
    # publish light value periodically (without using sleep)
    now = time.ticks_ms()
    distance = sensor.distance_cm()
    
    if distance<25:
        while not (25 < sensor.distance_cm() < 27):
            pass
        mqtt.publish(TOPIC_Ultrasonic, "1")
    
#     mqtt.publish(TOPIC_Ultrasonic, str(sensor.distance_cm()))
    time.sleep(1)
    
while True:
    distance = sensor.distance_cm()
    print('Distance:', distance, 'cm')
    time.sleep(1)
    
    

