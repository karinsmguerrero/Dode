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
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// definition of credentials and feeds


#define ssid            "Karina"
#define password        "karina98"
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

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Adafruit_PWMServoDriver pwmLED = Adafruit_PWMServoDriver(0x41);

// our servo # counter
uint8_t servonum = 12;
unsigned int pos0 = 150; // ancho de pulso en cuentas para pocicion 0°
unsigned int pos180 = 600; // ancho de pulso en cuentas para la pocicion 180°

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, SERVER, SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'test' for subscribing to changes.
const char TEST_FEED[] PROGMEM = USERNAME  PREAMBLE  "test";
Adafruit_MQTT_Subscribe tTest = Adafruit_MQTT_Subscribe(&mqtt, TEST_FEED);

// Setup a feed called 'test' for subscribing to changes.
const char star_FEED[] PROGMEM = USERNAME  PREAMBLE  "start";
Adafruit_MQTT_Subscribe tStart = Adafruit_MQTT_Subscribe(&mqtt, star_FEED);

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

//Orden: {DFA, AF, IFA, IFB,DFB} 
int ONE[5] = {6,5,4,3,2};
int TWO[5] = {6,1,3,9,10};
int THREE[5] = {8,9,2,1,4};
int FOUR[5] = {8,3,1,5,12};
int FIVE[5] = {4,1,6,11,12};
int SIX[5] = {10,11,5,1,2};
int SEVEN[5] = {10,9,8,12,11};
int EIGHT[5] = {4,12,7,9,3};
int NINE[5] = {8,7,10,2,3};
int TEN[5] = {6,2,9,7,11};
int ELEVEN[5] = {6,10,7,12,5};
int TWELVE[5] = {11,7,8,4,5};

int MOVEMENTS[12] = {11,10,6,1,3,8,2,9,7,12,4,5};

int current_face = 1;

bool executing = false;

char* commands;


void setup() {
     Serial.begin(115200);
  delay(10);
   pwm.begin();
  pwm.setPWMFreq(60); 
  delay(10);  
}
bool inicio = true;
void loop() {
    yield();
    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    //connectMQTT();
   /* move_motor(1);
    move_motor(2);
    move_motor(3);
    move_motor(4);
  delay(1000);*/
 /* Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(10))) {
        if(subscription == &tStart){*/
          for(int i = 0; i < 12; i++){
            current_face = MOVEMENTS[i];
            move_motor(MOVEMENTS[i]);
            delay(100);
          }
    /*}
    }*/
}


void move_motor(int channel){
    move_Angle(channel, 180);
    delay(900);
    move_Angle(channel, 20);
    delay(900);
    move_Angle(channel, 180);
    delay(800);
}
//prueba con angulos
void move_Angle(uint8_t channel, int ang) {
  int duty;
  duty=map(ang,0,180,pos0, pos180);
  pwm.setPWM(channel, 0, duty);  
}

