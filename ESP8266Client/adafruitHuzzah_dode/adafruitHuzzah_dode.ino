/***************************************************
  Adafruit MQTT Library ESP8266 Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ***************************************************
 *
 * Code by Andreas Spiess
  */
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define LED  15


// definition of credentials and feeds
 
#define ssid          "Martinez"
#define password      "jimmyantonio"
#define SERVER          "io.adafruit.com"
#define SERVERPORT      1883
#define MQTT_USERNAME   "karinamg"
#define MQTT_KEY        "d5165b2f551b4d23a0cc4a25edf1cced"
#define USERNAME          "karinamg/"
#define PREAMBLE          "feeds/"
#define T_TEST      "test"
#define T_CLIENTSTATUS    "clientStatus"
#define T_COMMAND         "command"


unsigned long entry;
bool clientStatus, prevClientStatus;
float luminosity, prevLumiosity;
unsigned long entryL, entryS;



#define userID         "karinamg"


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, SERVER, SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'test' for subscribing to changes.
const char TEST_FEED[] PROGMEM = USERNAME  PREAMBLE  "test";
Adafruit_MQTT_Subscribe tTest = Adafruit_MQTT_Subscribe(&mqtt, TEST_FEED);



//
void setup() {
  Serial.begin(115200);
  delay(100);
   pinMode(LED_BUILTIN, OUTPUT);



  connectWLAN();

  Serial.println("Connecting to MQTT server");
  entry = 0;
  Serial.begin(115200);
  delay(10);
  // connect to WiFi
  connectWLAN();

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&tTest);

  //Serial.println(USERNAME  PREAMBLE  T_COMMAND);

}

//
void loop() {
  yield();
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  connectMQTT();

  int hi = receiveCommand();
}

void connectWLAN() {
  // Connect to WiFi access point.
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
}


void connectMQTT() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}


int receiveCommand() {
  int clientSt = 99;
  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10))) {
    if (subscription == &tTest) {
      Serial.print(F("Got: "));
      Serial.println((char*)tTest.lastread);

      // Switch on the LED if an 1 was received as first character
      if (tTest.lastread[0]) {
        clientSt = true;
        digitalWrite(LED_BUILTIN, LOW);
        //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is acive low on the ESP-01)
        delay(3000);
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
  return clientSt;
}
