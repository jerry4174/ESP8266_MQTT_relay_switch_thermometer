//https://www.emqx.com/en/blog/esp8266-connects-to-the-public-mqtt-broker
// working version. Contact error in DHT22!

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Replace with your network credentials
const char *ssid =   "A1-3c0661";
const char *password =  "l24fq7ae3wyh";

// MQTT Broker settings
const char *mqtt_broker = "broker.emqx.io";  // EMQX broker endpoint
const char *mqtt_topic = "emqx/order";     // MQTT topic
const char *mqtt_response = "emqx/resp";     // MQTT response
const char *mqtt_username = "emqx";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

// DHT22 setup
#define DHTPIN D4  // Pin D4 connected to the DHT22 sensor
#define DHTTYPE DHT22  // DHT 22 (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);


#define PIN_DO         D0  // DO pin1 for relay (LED)
float temperature;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(PIN_DO, OUTPUT);
  digitalWrite(PIN_DO, LOW);
  Serial.begin(74880);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(mqttCallback);
}


// Function to connect to the WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTTBroker() {
    while (!client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            client.subscribe(mqtt_topic);
            // Publish message upon successful connection
            client.publish(mqtt_topic, "New EMQX connect!");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message = "";
  for (unsigned int i=0; i< length; i++) {
    message = message + (char)payload[i];
   }
    Serial.println(message);

    Serial.println("-----------------------");

     if( message.equals("rel1")) {
     digitalWrite(PIN_DO, HIGH);  // Turn the LED1 (relay1) on
     client.publish(mqtt_response, "rel1 ON!");
     }
 if ( message.equals("rel5")){  
     digitalWrite(PIN_DO, LOW); // Turn the LED1 (relay1) off
     client.publish(mqtt_response, "rel1 OFF!");
 }
 if( message.equals("temp1")){
      client.publish(mqtt_response, String(temperature).c_str());
  }
 //   client.publish(mqtt_response, message.c_str()); 
    
}

void loop() {
    if (!client.connected()) 
         connectToMQTTBroker();
    client.loop();

    
    temperature = dht.readTemperature();  //nemeri jinak ok
    Serial.println(temperature);
  //  client.publish(mqtt_response, String(temperature).c_str());
      

    
  delay(5000);
}




/*************
  
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any readings failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Publish temperature and humidity to MQTT topic
  String payload = "Temperature: " + String(temperature) + "°C  Humidity: " + String(humidity) + "%";
  Serial.println("Publishing data: " + payload);
  
  client.publish(mqtt_topic, payload.c_str());

  // Wait before sending the next reading
  delay(2000);
}
*******************/