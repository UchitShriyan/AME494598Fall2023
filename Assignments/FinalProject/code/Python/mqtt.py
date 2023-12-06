import paho.mqtt.client as mqtt

# MQTT Broker Settings
MQTT_BROKER = "54.158.32.40"  
MQTT_PORT = 1883  
MQTT_TOPIC = "gesture_topic"  

# Callback when connected to MQTT broker
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(MQTT_TOPIC)

# Callback when a message is received
def on_message(client, userdata, msg):
    print(f"Topic: {msg.topic} Message: {msg.payload.decode()}")

if __name__ == "__main__":
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(MQTT_BROKER, MQTT_PORT, 60)

    # Blocking call that processes network traffic, dispatches callbacks, and handles reconnecting.
    client.loop_forever()
