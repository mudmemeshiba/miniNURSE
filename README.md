#  miniNURSE -`♡´-

Nowadays we are entering an aging society. Many elderly people who are having health issues stay at home independently, often without the supervision of their families or caregivers. They usually face some unwanted situations such as forgetting to take a medicine or neglecting to record their blood pressure result for the doctor visits. Thus, we, ไม่ได้เป็นหมอแต่เป็นห่วง (not a doctor by name, but a healer by heart), have integrated these problems into our hardware development project called *miniNURSE*.

*miniNURSE* is a multi-purpose scheduler device. Currently, there are 3 functions:

1) Medicine reminder: enables users to schedule a time for medicine intake via Node-RED dashboard from anywhere. When it's time, the buzzer will buzz the loop of frequencies as music. It will not stop unless the medicine is taken out (detected with LDR sensor inside the opaque medicine box)
PS: We will assume that if the medicine is taken out, it will be later consumed.

2) Miscellaneous reminder: similar to medicine reminder, but the buzzer will not loop

3) Blood pressure monitor: when the user finishes checking their blood pressure, the equipped camera nearby will capture the screen's result and send it to Line via Line notify.

Although this project originated from the needs of older people, anyone can use miniNURSE. For example:
- Schedule the time to take a break from work to prevent overloading or office syndrome
- Schedule the time to go for a walk with your lovely pets
- Schedule any random time to enjoy the music from the buzzer
- etc.

Our goal is to universally enhance everyone's quality of life.

## Directory tree

```txt
miniNURSE
├── LICENSE.txt                        # MIT License
├── Node-RED
│   └── miniNURSE-flow.json            # Dashboard and UI Control flow
├── README.md                          # README
├── board
│   ├── esp32-camera
│   │   ├── config.h                   # configurations for esp32-camera
│   │   └── esp32-camera.ino           # takes pictures, receives MQTT from ultra-mqtt.py and send the pictures via Line Notify
│   ├── notify
│   │   ├── buzzer.h                   # buzzer variables and frequencies storage
│   │   ├── config.h                   # configurations for notification board
│   │   ├── notify.ino                 # main sketch
│   │   ├── parseJson.cpp              # receives and parses JSON data from Node-RED via MQTT
│   │   └── parseJson.h                # configurations for parseJson.cpp 
│   └── ultra-mqtt
│       └── ultra-mqtt.py              # ultrasonic sensor and timer 
├── line-notify
│   └── blood-pressure-result.jpg      # line notification example
└── schematic
    ├── Schematic_Buzzer_LDRs_OLED.png
    ├── Schematic_Ultrasonic.png
    └── camera_circuit.png
```

## Hardware specification
ESP32-S3-DEVKITC-1 (2x) 
### 1st Board: Notification board
- MH-FMD Passive Buzzer (1x) 
    - Buzzes notification frequencies -> music!
- OLED (1x) 
    - Displays details inputted data from Node-RED
- LDR sensor (1x) 
    - Measures light intensity inside the medicine box
### 2nd Board: Distance detector board
- HC-SR04 Ultrasonic sensor (1x) 
    - Measures distance from the user's arm
### ESP32-camera module (1x) 
- Captures blood pressure result's screen
<img src="https://github.com/mudmemeshiba/miniNURSE/assets/98101484/1e3ae948-edcc-4125-a7e0-80afa704244f" height="327">

## Libraries used 
#### C / C++ 
- Notification board
    - `Arduino.h` Arduino's core header file
    - `Adafruit_GFX.h` provides graphics functions
    - `Adafruit_SSD1306.h` controls SSD1306 OLED
    - `Wire.h` allows ESP32S3 to communicate with other devices via I2C
    - `WiFi.h` connects the ESP32S3 with WiFi
    - `WiFiUdp.h` provides UDP Interface for `WiFi.h`
    - `NTPClient.h` retrives the time from an NTP server
    - `PubSubClient.h` establishes connection with mqtt
    - `ArduinoJson.h` parses and generates JSON data
- ESP-Camera
    - `TridentTD_LineNotify` sends picture via Line Notify
    - `esp_camera` drives the esp32 camera
#### Python
- Ultrasonic board
    - `HCSR04` supports HC-SR04 Ultrasonic distance sensor
    - `time` includes time related fuctions

## Functionality of the program

### Scheduler
The users themselves or their caregivers input their data and desired time (24-hour format) to the Node-RED form (one schedule at a time). The data is store on the server directory ./data/ in a text file and can be access by http request. Then it will be displayed on the UI table unless the `clear table` button is clicked.  Every time there is a new input, the file is overwritten.

We use timestamp to read the file and send JSON data to the ESP32 via MQTT. Currently, the timestamp is triggered every 1 minute to ensure that the program will be scheduled only one time in one minute. The JSON data is then parsed and displayed on OLED screen.

For the medicine scheduler, When it's time, the buzzer will continuously play the music while the LDR sensor detects the light level inside the medicine box. Unless the light level is more than 60%, the buzzer will not stop playing. Meanwhile, the misc scheduler does not require any action to stop the music.

### Blood Pressure Monitor
The ultrasonic sensor is placed around 25cm away from the wall. If the user places the arm next to the blood pressure device, the sensor will detect changes in distance and start the 1 minute countdown (the average time of ) before the ESP32-camera captures the result. The MQTT is then sent from the ultrasonic board to the camera board. Finally, the picture will be notified on line.

Line Notify bot can be invited to any chat or group. Therefore, more than one Line account can view it.

### Node-RED Dashboard (user input)
![node_red](https://github.com/mudmemeshiba/miniNURSE/assets/98101484/c637aea6-e471-4e4f-9c77-9c7d5edc9f40)
#### Data that must be filled into the form
- Medicine scheduler (FirstBox): Medicine name, Amount, Hour, Minute, details

OR
- Misc scheduler (SecondBox): Activity, Hour, Minute

### Node-RED Flow
<img width="567" alt="Screenshot 2024-03-15 055221" src="https://github.com/mudmemeshiba/miniNURSE/assets/133182776/e7bb6113-b5d3-4dcf-a035-012800c8f480">
<img width="337" alt="Screenshot 2024-03-14 104124" src="https://github.com/mudmemeshiba/miniNURSE/assets/133182776/7f860e0f-b759-478a-9346-225f3eb52a78">


### Innovated by ไม่ได้เป็นหมอแต่เป็นห่วง (CPE37, KU83)

- 6610501963  KRITTIWIT     KAMPRADAM 

- 6610502153  PACHARAMON    PUTRASRENI 

- 6610505527  PENPITCHA     JUMRUSPUN 

- 6610505586  SUPATHICHARAT CHALERMKWANMONGKOL

This project is under the 01204114 Introduction to Hardware Development Course, 2/2023.

Department of Computer Engineering, Faculty of Engineering, Kasetsart University
