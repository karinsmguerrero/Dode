import paho.mqtt.client as mqtt
import time

class MQTTClient:

    #--------------------- CONSTRUCTOR ------------------------------------
    def __init__(self):
        # ------------------------ PARÄMETROS DE CONEXIÖN ----------------------------------------

        self.MQTT_broker = "io.adafruit.com"
        self.IO_USERNAME = "karinamg"
        self.IO_KEY = "d5165b2f551b4d23a0cc4a25edf1cced"
        self.IO_TOPIC = "karinamg/feeds/commands"

        self.client = mqtt.Client("DodeFast")
        self.client.username_pw_set(self.IO_USERNAME, password=self.IO_KEY)

    def connect(self):
        print("conectando con el broker: ", self.MQTT_broker)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_log = self.on_log
        self.client.connect(self.MQTT_broker, 1883)

    def on_log(self, client, userdata, level, buf):
        print("LOG: " + buf)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("Conexión exitosa")
        else:
            print("Conexión fallida = ", rc)

    def on_disconnect(self, client, userdata, flags, rc=0):
        print("Disconnected result code: " + str(rc))

    def publishMessage(self, data):
        self.connect()
        self.client.loop_start()
        time.sleep(1)
        self.client.publish(self.IO_TOPIC, str(data))
        self.client.loop_stop()
        self.client.disconnect()


Client = MQTTClient()
Client.publishMessage(["AF", "AF", "F", "F"])
