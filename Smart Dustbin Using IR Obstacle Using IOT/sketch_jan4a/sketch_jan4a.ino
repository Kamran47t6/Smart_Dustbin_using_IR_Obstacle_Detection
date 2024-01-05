#include <WiFi.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>

const char *ssid = "Kami";
const char *password = "Someone56";
const char *mqttServer = "test.mosquitto.org";  
const int mqttPort = 1883;
const char *mqttClientId = "COAL_GROUP_8_A";
const char *outputTopic = "esp32/output_53";
const char *inputTopic = "esp32/input_53";
static unsigned long lastMillis = 0;

WiFiClient espClient;
PubSubClient client(espClient);

// ThingSpeak settings
unsigned long channelID = 2395229;
const char *writeAPIKey = "CGGRUSO3CDXQX4DJ";

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  WifiSetup();

  // Configure MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callBack);
  connectToMQTT();

  // Initialize ThingSpeak
  ThingSpeak.begin(espClient);
}

void WifiSetup() {
  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected with IP address: ");
  Serial.println(WiFi.localIP());
}

void callBack(char *inputTopic, byte *message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(inputTopic);
  Serial.print(". Message : ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);
  Serial.print("Message Sent to UART : ");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WifiSetup();
  }
  if (Serial2.available() > 0) {
    // String receivedChar = Serial2.readString();
    // Serial.print("Message Received through UART  :   ");
    // Serial.println(receivedChar);
  }

  // Handle MQTT events
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Message Publishing on app with delay of 5s
  if (millis() - lastMillis > 5000) {

    String resultString = " working  ";
    const char *message = resultString.c_str();
    publishMessage(message);

    // Send data to ThingSpeak
    sendToThingSpeak();

    lastMillis = millis();
  }

}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(mqttClientId)) {
      Serial.println("Connected to MQTT");
      client.subscribe(inputTopic);
    } else {
      Serial.print("Failed with state   :  ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void publishMessage(const char *message) {
  if (client.connected()) {
    client.publish(outputTopic, message);
    Serial.print("Message Published :  ");
    Serial.println(message);
  }
}

void sendToThingSpeak() {
  // Create data for Field1 and Field2
  int valueField1 = random(100); 
  int valueField2 = random(100); 

  // Write to ThingSpeak
  ThingSpeak.writeField(channelID, 1, valueField1, writeAPIKey);
  ThingSpeak.writeField(channelID, 2, valueField2, writeAPIKey);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);
  
  if (status == 200) {
    Serial.println("ThingSpeak update successful!");
  } else {
    Serial.println("Error updating ThingSpeak. HTTP status code: " + String(status));
  }
}