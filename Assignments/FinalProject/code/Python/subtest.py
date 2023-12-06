import paho.mqtt.client as mqtt
import json

# MQTT parameters
broker_address = "54.196.157.83"  # Use the private IP of AWS instance
broker_port = 1883
topic = "sensor/data"

# The callback for when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(topic)

# The callback for when a PUBLISH message is received from the server
def on_message(client, userdata, message):
    print("Message received: ", str(message.payload.decode("utf-8")))
    payload_data = json.loads(message.payload)
    print("Data:", payload_data)

# Create a MQTT client instance
client = mqtt.Client("Subscriber")

# Assign event callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker_address, broker_port)

# Start the loop
client.loop_forever()

