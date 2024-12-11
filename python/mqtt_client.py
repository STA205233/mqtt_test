#! /usr/bin/env python3
import paho.mqtt.client as mqtt
from paho.mqtt.enums import CallbackAPIVersion


def on_connect(client, userdata, flag, rc, properties=None):
    print(f"Connected with result code {rc}")


def on_message(client, userdata, msg):
    print(f"Topic: {msg.topic}, PayLoad: {msg.payload}, QOS: {msg.qos}")


def on_publish(client, userdata, mid, rc, properties):
    print(f"Message {mid} published")


if __name__ == "__main__":
    ip = "localhost"
    port = 1883
    client = mqtt.Client(callback_api_version=CallbackAPIVersion.VERSION2)
    client.connect(ip, port, 60)
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_publish = on_publish
    client.subscribe("test")
    client.loop_forever()
