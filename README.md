# XCSoar-remote-stick
Remote control of XCSoar via Bluetooth by ESP32

Videos:
https://www.youtube.com/playlist?list=PL9kPSX54kefuFYvbrconY-khqX8SFhgzt

Photos:
https://www.zonerama.com/jelito/Album/6922525

XCsoar forum thread for discussion:
https://forum.xcsoar.org/viewtopic.php?f=3&t=4205

____________________

### Input event file (XCI file) for XCSoar
The input event file (extension .xci) is a plain text file designed to control the input and events in your XCSoar glide computer. Upload this file to your xcsoar_data folder and select this file in XCSoar Config -> System -> Look -> Language, Input -> Events.

____________________

### Arduino sketch (file A7_remote_stick.ino)
This first small and simple arduino sketch comes with full functionality of buttons control of XCSoar.

##### Settings in Arduino IDE:
  - Board: M5Stick-C

____________________

### Arduino sketch with OTA (file A7_remote_stick_OTA_embedded_jQuery.ino)
This arduino sketch comes with buttons control of XCSoar and also with OTA web updater, which enables wirelessly updating ESP32 via web server. To start webserver only turn on remote stick with pressed top button (fn). Then connect with your PC to wifi hotspot created by remote stick and in your webbrowser go to http://192.168.4.1/. Now you can here wirelessly upload new compiled binary from arduino sketch. 

Unfortunately this sketch is quite large, you must change partition scheme of ESP32 to sketch can fit tu ESP32 memory.

##### Settings in Arduino IDE:
  - Board: M5Stick-C
  - Partition Scheme: Minimal SPIFFS (Large APPS with OTA)
