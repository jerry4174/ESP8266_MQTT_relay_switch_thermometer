/****************************************************
Jerry was here: 
ESP8266_MQSS_client.ino   
    V 1.0   31.07.2023  Basic working version 
    V 1.1   22.08.2023  Improve of reconnect() function.
                        Publish random clientId in reconnect() function.
                        Write a publish response from the callback function (with another topic). 
                        After start should be all pins OFF!    
    V 1.2   01.09.2023  Switch with 4 relais with a response.
    V 1.3   23.11.2023  Switch on/off by BUILTIN_LED repaired.    

Setup: Board NodeMCU 1.0 (ESP-12E...), 115200 baud, 
Preferences -> Additional Board Manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Libraries: PubSubClient by Nick O'Leary
Serial Port: linux /dev/tty0, win COM6 (from Device Manager -> Ports COM & LPT -> USB to UART Bridge (COM6)) 
Client mobile app IoT MQTT.
broker.emqx.io, 1883 TCP, no user/passw, topics "order" and "resp"

Problems: 
         - Serial Monitor wrong characters or no upload - try another usb cabel with data lines! ok
         - Many reconnects in the night (e.g. 190): The problem is MY wifi connection to the broker.
           Set router to wifi channel 11. Working well with wifi channel 11! Or reboot the router. ok
      

To do:
           Remote thermometer


*****************************************/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =   "A1-3c0661";
const char *password =  "l24fq7ae3wyh";

   // MQTT Broker
  // const char *mqtt_server = "broker.hivemq.com";
   const char *mqtt_server = "broker.emqx.io";
   //const char *mqtt_server = "broker.mqtt-dashboard.com";
   const int mqtt_port = 1883;
   const char *topic =   "order";   // your topic for commands
   const char *resp =   "resp";     // your topic for response
   char val_str[20];
   char id_str[40];
   int value = 0;
  
   WiFiClient espClient;
   PubSubClient client(espClient);
   
/**** LED Settings *******/
#define PIN_REL_1         D1  
#define PIN_REL_2         D2  
#define PIN_REL_3         D3  
#define PIN_REL_4         D5  // D4 didn't work

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  payload[length] = 0;        // convert payload to String
  String recv_payload = String(( char *) payload);
//  Serial.println(recv_payload );
//  Serial.println();
  
// 4 relais switch on and off. On board LED lights, if any relais is ON.
// Relais 1 ON
if (strncmp((char*)payload, "rel1",length) == 0) {  
   digitalWrite(PIN_REL_1, HIGH);
   digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
   client.publish(resp, "rel1 ON");
   Serial.println("rel1 ON");
}
// Relais 2 ON
if (strncmp((char*)payload, "rel2",length) == 0) { 
   digitalWrite(PIN_REL_2, HIGH); 
   digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
   client.publish(resp, "rel2 ON");
   Serial.println("rel2 ON");
}
// Relais 3 ON   
if (strncmp((char*)payload, "rel3",length) == 0) { 
     digitalWrite(PIN_REL_3, HIGH); 
     digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
     client.publish(resp, "rel3 ON");
     Serial.println("rel3 ON");
}
// Relais 4 ON
if (strncmp((char*)payload, "rel4",length) == 0) { 
     digitalWrite(PIN_REL_4, HIGH); 
     digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
     client.publish(resp, "rel4 ON");
     Serial.println("rel4 ON");
} 
// Relais 1 OFF
if (strncmp((char*)payload, "rel5",length) == 0) {
   digitalWrite(PIN_REL_1, LOW);
   client.publish(resp, "rel1 OFF");
   Serial.println("rel1 OFF");
}
// Relais 2 OFF
if (strncmp((char*)payload, "rel6",length) == 0) {
   digitalWrite(PIN_REL_2, LOW);    digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
   client.publish(resp, "rel2 OFF");
   Serial.println("rel2 OFF");
}
// Relais 3 OFF   
if (strncmp((char*)payload, "rel7",length) == 0) {
   digitalWrite(PIN_REL_3, LOW);   off
   client.publish(resp, "rel3 OFF");
   Serial.println("rel3 OFF");
}
// Relais 4 OFF
if (strncmp((char*)payload, "rel8",length) == 0) {
   digitalWrite(PIN_REL_4, LOW);   
  
   client.publish(resp, "rel4 OFF");
   Serial.println("rel4 OFF");
} 
// 1 or more PIN_REL are on (HIGH) BUILTIN_LED is on
  if(digitalRead(PIN_REL_1) || digitalRead(PIN_REL_2) || digitalRead(PIN_REL_3) || digitalRead(PIN_REL_4) == HIGH) {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the on-board LED on
      }
  else  // all PIN_REL are off (LOW) BUILTIN_LED is off
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the on-board LED off;
}

void reconnect() {
  // Loop until we're reconnected
  value++;
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection..."); 
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.print("My client connected id: ");
      Serial.print(clientId);
      Serial.print(", ");
      Serial.println(value);
      // Once connected, publish an announcement...
      
      sprintf(val_str, "%d", value);      // number to string
      clientId.toCharArray(id_str, 40);   // C++ String to string
      client.publish(topic, "My MQSS Client connected id: ");
      client.publish(topic, id_str);
      client.publish(topic, val_str);
      

        // ... and resubscribe
      client.subscribe(topic);
      client.subscribe(resp);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());   // -2 connect failed, -4 timeout
      Serial.println(" try again in 5 seconds");
           // Wait 5 seconds before retrying
     delay(5000);
    }
  }
}

void setup() {
  
  pinMode(PIN_REL_1, OUTPUT); 
  digitalWrite(PIN_REL_1, LOW);  // LED is OFF!
  pinMode(PIN_REL_2, OUTPUT); 
  digitalWrite(PIN_REL_2, LOW);  // LED is OFF!
  pinMode(PIN_REL_3, OUTPUT); 
  digitalWrite(PIN_REL_3, LOW);  // LED is OFF!
  pinMode(PIN_REL_4, OUTPUT);
  digitalWrite(PIN_REL_4, LOW);  // LED is OFF!

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);  // LED is OFF!
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
   delay(5000);
}
