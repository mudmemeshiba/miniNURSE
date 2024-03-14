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

## Hardware
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
  ![blood-pressure-result](https://github.com/mudmemeshiba/miniNURSE/assets/98101484/1e3ae948-edcc-4125-a7e0-80afa704244f)



## Node-RED

### Dashboard (user input)

![node_red](https://github.com/mudmemeshiba/miniNURSE/assets/98101484/c637aea6-e471-4e4f-9c77-9c7d5edc9f40)



## Software
### esp-camera
- PubSubClient
    - for connecting with mqtt
- TridentTD_LineNotify
    - to sending to line notify
- esp_camera
    - to connect the camera

### Flow



## Innovated by ไม่ได้เป็นหมอแต่เป็นห่วง (CPE37, KU83)

- 6610501963 KRITTIWIT    KAMPRADAM 

- 6610502153 PACHARAMON   PUTRASRENI 

- 6610505527 PENPITCHA    JUMRUSPUN 

- 6610505586 ศุภธิชารัตน์	เฉลิมขวัญมงคล

This project is a under the 01204114 Introduction to Hardware Development Course, 2/2023.

Department of Computer Engineering, Faculty of Engineering, Kasetsart University
