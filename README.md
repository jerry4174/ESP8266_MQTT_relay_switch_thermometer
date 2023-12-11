# ESP8266_MQTT_relay_switch
MQTT Relay switch with ESP8266. Remote connection without static IP.

## Project Description
Remote measure of temperature and humidity and  
remote control of 4 relais with Message Queuing Telemetry Transport (MQTT).

## Purpose
Remote switching of 4 electrical devices. Also you can switch on/off a heating. 

## Built With  
ESP8266  
Relais JQC3F 5V DC  
Thermometer DHT22  
EMXQ MQTT Broker
Mobile MQTT Client IoT MQTT (Android)
PC MQTT Client MQTT Explorer (Windows)


## Getting Started  
Connect device as described in the wiring diagram 1.JPG.  
Change your wifi parameters in the ESP8266 program termo_NAC_relais.ino.  
Load the program to the ESP8266, check the wifi connection.  
Test the function of the thermometer in monitor.  
Test the wifi connection first local, then remote.  
Use adapted index - anonym.php file to see results in a browser. 
