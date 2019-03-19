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
#include <iostream>
#include <cstring>
#include <sstream>

// definition of credentials and feeds
 
#define ssid            "Martinez"
#define password        "jimmyantonio"
#define SERVER          "io.adafruit.com"
#define SERVERPORT      1883
#define MQTT_USERNAME   "karinamg"
#define MQTT_KEY        "d5165b2f551b4d23a0cc4a25edf1cced"
#define USERNAME        "karinamg/"
#define PREAMBLE        "feeds/"
#define T_TEST          "test"
#define T_CLIENTSTATUS  "status"
#define T_COMMAND       "command"
#define userID          "karinamg"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, SERVER, SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'test' for subscribing to changes.
const char TEST_FEED[] PROGMEM = USERNAME  PREAMBLE  "test";
Adafruit_MQTT_Subscribe tTest = Adafruit_MQTT_Subscribe(&mqtt, TEST_FEED);

// Setup a feed called 'commands' for subscribing to changes.
const char COMMANDS_FEED[] PROGMEM = USERNAME  PREAMBLE  "commands";
Adafruit_MQTT_Subscribe tCommands = Adafruit_MQTT_Subscribe(&mqtt, COMMANDS_FEED);

// Setup a feed called 'Status' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char CLIENTSTATUS_FEED[] PROGMEM = USERNAME  PREAMBLE  "state";
Adafruit_MQTT_Publish tClientStatus = Adafruit_MQTT_Publish(&mqtt, CLIENTSTATUS_FEED);

// Setup a feed called 'Status' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char AVAILABILITY_FEED[] PROGMEM = USERNAME  PREAMBLE  "availability";
Adafruit_MQTT_Publish tAvailability = Adafruit_MQTT_Publish(&mqtt, AVAILABILITY_FEED);


void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Connecting to MQTT server");
  Serial.begin(115200);
  delay(10);
  // connect to WiFi
  connectWLAN();

  // Setup MQTT subscription for feed.
  mqtt.subscribe(&tTest);
  mqtt.subscribe(&tCommands);

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
    publishAvailabilityStatus(false);
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  publishAvailabilityStatus(true);
  Serial.println("MQTT Connected!");
}

//Retorna true si pudo publicar
bool publishClientStatus(bool clientStat) {
  bool success = false;
  Serial.print(F("\nSending clientStatus "));
  Serial.print(clientStat);
  Serial.print("...");

  if (! tClientStatus.publish(clientStat, 1)) Serial.println(F("Failed"));
  else {
    Serial.println(F(" OK!"));
    success = true;
  }
  return success;
}

bool publishAvailabilityStatus(bool available) {
  bool success = false;
  Serial.print(F("\nSending availability status"));
  Serial.print(available);
  Serial.print("...");

  if (! tAvailability.publish(available, 1)) Serial.println(F("Failed"));
  else {
    Serial.println(F(" OK!"));
    success = true;
  }
  return success;
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
        Serial.print("MQTT Message: ");
        Serial.println(tTest.lastread[0]);
        Serial.println(tTest.lastread[1]);
        Serial.println(tTest.lastread[2]);
        clientSt = true;
        digitalWrite(LED_BUILTIN, LOW);
        //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is acive low on the ESP-01)
        delay(3000);
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    else if(subscription == &tCommands){
      Serial.print(F("Got: "));
      char* commands = (char*)tCommands.lastread
      publishAvailabilityStatus(false);
       int i = 0;
    std::string commmand;
    while (commands[i] != '\0'){
        if(commands[i] == '\"'){
            i++;
            std::ostringstream os;
            while (commands[i] != '\"'){
                os << commands[i];
                i++;
            }
            commmand = os.str();
            i++;
            Serial.print("Command: " + command);
            if(command == "A"){
              AF();
            }
            else if(command == "F"){
              F();
            }
        }
        i++;
    }
      
  }
  return clientSt;
}

void AF(){
    Serial.println("Move AF");
    }

void F(){
  Serial.println("Move F");
  }

void DFA(){
    Serial.println("Move DFA");
  }

void IFA(){
    Serial.println("Move IFA");
  
}

void DFB(){
      Serial.println("Move DFB");
}

void IFB(){
      Serial.println("Move IFB");
}

void A(){
      Serial.println("Move A");
}

void DAA(){
      Serial.println("Move DAA");
}

void IAA(){
      Serial.println("Move IAA");
}

void DAB(){
      Serial.println("Move DAB");
}

void IAB(){
      Serial.println("Move IAB");
}

void AA(){
      Serial.println("Move AA");
}

