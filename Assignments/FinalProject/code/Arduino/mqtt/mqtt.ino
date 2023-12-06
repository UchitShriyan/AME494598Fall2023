#include <Wire.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiNINA.h>

const char* mqttServer = "54.196.157.83";
const char* ssid = "Hell";
const char* password = "nosecurity";

WiFiClient espClient;
PubSubClient client(espClient);

void receiveEvent(int howMany);

void setup() {
  Serial.begin(115200);
  Wire.begin(9); // Join the I2C bus as a slave at address 9
  Wire.onReceive(receiveEvent); // Register the receive event function
  Serial.println("I2C Receiver Ready");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Connect to MQTT Broker
  client.setServer(mqttServer, 1883);
  client.setCallback(mqttCallback); // Set MQTT callback
}

int lastGesture = -1;
bool newGestureAvailable = false;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check if a new gesture is available to publish
  if (newGestureAvailable) {
    // Create JSON object
    StaticJsonDocument<200> doc;
    doc["gesture"] = lastGesture;

    char jsonBuffer[200];
    serializeJson(doc, jsonBuffer);

    Serial.print("Sending JSON: ");
    Serial.println(jsonBuffer);

    // Attempt to publish
    if (client.publish("sensor/data", jsonBuffer)) {
      Serial.println("Publish successful");
    } else {
      Serial.println("Publish failed");
    }
    newGestureAvailable = false; // Reset flag
  }
}

void receiveEvent(int howMany) {
  if (howMany >= 2) {
    int lowerByte = Wire.read();
    int upperByte = Wire.read() << 8;
    lastGesture = upperByte | lowerByte;
    Serial.print("Received Integer: ");
    Serial.println(lastGesture);

    newGestureAvailable = true;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming messages here
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}



// #include <Wire.h>

// void receiveEvent(int howMany);

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(9); // Join the I2C bus as a slave at address 9
//   Wire.onReceive(receiveEvent); // Register the receive event function
//   Serial.println("I2C Receiver Ready");
// }

// void loop() {
//   // Main code here (nothing needed for I2C receiving)
// }

// void receiveEvent(int howMany) {
//   if (howMany >= 2) {
//     // Read two bytes and combine them into an integer
//     int lowerByte = Wire.read(); // Read the first (lower) byte
//     int upperByte = Wire.read(); // Read the second (upper) byte
//     upperByte <<= 8;             // Shift the upper byte to the correct position
//     int receivedInt = upperByte | lowerByte; // Combine the two bytes

//     Serial.print("Received Integer: ");
//     Serial.println(receivedInt);
//   }
// }
