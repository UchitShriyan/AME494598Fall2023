#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  

// WiFi and MQTT settings
const char* ssid = "Hell";
const char* password = "nosecurity";
const char* mqtt_server = "3.80.60.182";

// Motor control pins
const int INLF3 = D11;
const int INLF4 = D10;
const int INRF3 = D3;
const int INRF4 = D2;
const int INLB1 = D5;
const int INLB2 = D4;
const int INRB1 = D1;
const int INRB2 = D0;

// Global variables
int motorSpeed = 512; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}

void controlMotors(int gesture) {
    // Include the motor control logic here, based on the gesture value
    switch (gesture) {
        case 0: // Backward
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 255);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 255);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 255);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 255);

            delay(500);
            break;

        case 1: // Left Diagonal Backward
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 255);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 255);

            delay(500);
            break;

        case 2: // Right Diagonal Backward
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 255);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 255);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 0);

            delay(500);
            break;

        case 3: // Left Diagonal Forward
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 255);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 255);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 0);

            delay(500);
            break;

        case 4: // Right Diagonal Forward
            analogWrite(INLF3, 255);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 255);
            analogWrite(INRB2, 0);

            delay(500);
            break;

        case 5:  //Stop
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 0);
            delay(500);
            break;


        case 6: // Forward
            analogWrite(INLF3, 255);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 255);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 255);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 255);
            analogWrite(INRB2, 0);

            delay(500);
            break;

        case 7: // Left
            analogWrite(INLF3, 0);
            analogWrite(INLF4, 255);

            analogWrite(INRF3, 255);
            analogWrite(INRF4, 0);

            analogWrite(INLB1, 255);
            analogWrite(INLB2, 0);

            analogWrite(INRB1, 0);
            analogWrite(INRB2, 255);

            delay(500);
            break;

        case 8: // Right
            analogWrite(INLF3, 255);
            analogWrite(INLF4, 0);

            analogWrite(INRF3, 0);
            analogWrite(INRF4, 255);

            analogWrite(INLB1, 0);
            analogWrite(INLB2, 255);

            analogWrite(INRB1, 255);
            analogWrite(INRB2, 0);

            delay(500);
            break;        
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload, length);
    int gesture = doc["gesture"];
    controlMotors(gesture);
    Serial.println(gesture);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("sensor/data");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    // Initialize motor control pins
    pinMode(INLF3, OUTPUT);
    pinMode(INLF4, OUTPUT);
    pinMode(INRF3, OUTPUT);
    pinMode(INRF4, OUTPUT);
    pinMode(INLB1, OUTPUT);
    pinMode(INLB2, OUTPUT);
    pinMode(INRB1, OUTPUT);
    pinMode(INRB2, OUTPUT);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
