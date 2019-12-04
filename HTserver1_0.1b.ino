#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

const char* ssid = "YOUR_SSID_NETWORK";
const char* password = "YOUR_SSID_PASSWORD";
char* topic = "YOUR_CALLBACK_TOPIC";
char* topicTemp = "YOUR_TEMP_TOPIC";
char* topicHum = "YOUR_HUM_TOPIC";
char* server = "YOUR_MQTT_SERVER_ADDRESS";
const char* mqtt_user = "YOUR_MQTT_SERVER_USERNAME";      
const char* mqtt_password = "YOUR_MQTT_SERVER_PASSWORD";   
String clientName = "YOUR_CLIENT_NAME"; //MUST BE UNIQUE

#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE,15);

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int length) {

  String message = (char*)payload;
  Serial.println(message);
  
}
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
Serial.begin(115200);
delay(10);
dht.begin();
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

Serial.print("Connecting to ");
Serial.print(server);
Serial.print(" as ");
Serial.println(clientName);


if (client.connect((char*) clientName.c_str(),mqtt_user, mqtt_password)) {
Serial.println("Connected to MQTT broker");
Serial.print("Topic is: ");
Serial.println(topic);

}
else {
Serial.println("MQTT connect failed");
Serial.println("Will reset and try again...");
abort();
}
}

void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}


static int counter = 0;
String payload ;
String temp;
String hum;
payload += t;
payload += ":";
payload += h;
temp += t;
hum += h;

if (client.connected()){
Serial.print("Sending payload: ");
Serial.println(payload);

if (client.publish(topicTemp, (char*) temp.c_str()))
{
Serial.println("Publish ok");
}
else {
Serial.println("Publish failed");
}
if (client.publish(topicHum, (char*) hum.c_str()))
{
Serial.println("Publish ok");
}
else {
Serial.println("Publish failed");
}

}
else {
if (client.connect((char*) clientName.c_str())) {
Serial.println("Connected to MQTT broker");
Serial.print("Topic is: ");
Serial.println(topic);
}
}
++counter;
delay(5000);
}
