#! /usr/bin/env python3
import paho.mqtt.client as mqtt
from time import sleep
from paho.mqtt.enums import CallbackAPIVersion
from mqtt_client import on_connect, on_message, on_publish

if __name__ == "__main__":
    ip = "localhost" # Change to the IP address of the MQTT broker
    port = 1883 # Change to the port of the MQTT broker
    client = mqtt.Client(CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_publish = on_publish
    client.on_message = on_message
    client.connect(ip, port, 60)
    message = input("Enter message: ") # Enter the message to be published
    client.loop_start()

    while True:
        try:
            client.publish("test", message, qos=1)
            sleep(1)
        except KeyboardInterrupt:
            break
    client.loop_stop()
