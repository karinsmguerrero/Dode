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
/*#define ssid            "Karina"
#define password        "karina98"*/
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

int current_face = 1;

int LED_ONE = 2;
int LED_TWO = 3;
int LED_THREE = 4;
int LED_FOUR = 5;
int LED_FIVE = 6;
int LED_SIX = 7;
int LED_SEVEN = 8;
int LED_EIGHT = 9;
int LED_NINE = 10;
int LED_TEN = 11;
int LED_ELEVEN = 12;
int LED_TWELVE = 13;


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
    /*
    pinMode(LED_ONE, OUTPUT);
     pinMode(LED_TWO, OUTPUT);
      pinMode(LED_THREE, OUTPUT);
       pinMode(LED_FOUR, OUTPUT);
        pinMode(LED_FIVE, OUTPUT);
         pinMode(LED_SIX, OUTPUT);
          pinMode(LED_SEVEN, OUTPUT);
           pinMode(LED_EIGHT, OUTPUT);
            pinMode(LED_NINE, OUTPUT);
             pinMode(LED_TEN, OUTPUT);
              pinMode(LED_ELEVEN, OUTPUT);
               pinMode(LED_TWELVE, OUTPUT);
               */
}

void loop() {
    yield();
    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    connectMQTT();
    receiveCommand();

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
       
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
    }
    publishAvailabilityStatus(true);
    publishClientStatus(true);
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
    Serial.print(F("\nSending availability status: "));
    Serial.print(available);
    Serial.print("...");

    if (! tAvailability.publish(available, 1)) Serial.println(F("Failed"));
    else {
        Serial.println(F(" OK!"));
        success = true;
    }
    return success;
}

void AF(){
  if(current_face == 1){
    current_face = ONE[1];
    //Move_motor(current_face);
  }else if(current_face == 2){
    current_face = TWO[1];
    //Move_motor(current_face);
  }else if(current_face == 3){
    current_face = THREE[1];
    //Move_motor(current_face);
  }else if(current_face == 4){
    current_face = FOUR[1];
    //Move_motor(current_face);
  }else if(current_face == 5){
    current_face = FIVE[1];
    //Move_motor(current_face);
  }else if(current_face == 6){
    current_face = SIX[1];
    //Move_motor(current_face);
  }else if(current_face == 7){
    current_face = SEVEN[1];
    //Move_motor(current_face);
  }else if(current_face == 8){
    current_face = EIGHT[1];
    //Move_motor(current_face);
  }else if(current_face == 9){
    current_face = NINE[1];
    //Move_motor(current_face);
  }else if(current_face == 10){
    current_face = TEN[1];
    //Move_motor(current_face);
  }else if(current_face == 11){
    current_face = ELEVEN[1];
    //Move_motor(current_face);
  }else if(current_face == 12){
    current_face = TWELVE[1];
    //Move_motor(current_face);
  }
    Serial.println("Move AF");/*
     digitalWrite(LED_ONE, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_ONE, LOW); */
}

void F_MOVE(){
    Serial.println("Move F");/*
    digitalWrite(LED_TWO, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_TWO, LOW); */
}

void DFA(){
    if(current_face == 1){
    current_face = ONE[0];
    //Move_motor(current_face);
  }else if(current_face == 2){
    current_face = TWO[0];
    //Move_motor(current_face);
  }else if(current_face == 3){
    current_face = THREE[0];
    //Move_motor(current_face);
  }else if(current_face == 4){
    current_face = FOUR[0];
    //Move_motor(current_face);
  }else if(current_face == 5){
    current_face = FIVE[0];
    //Move_motor(current_face);
  }else if(current_face == 6){
    current_face = SIX[0];
    //Move_motor(current_face);
  }else if(current_face == 7){
    current_face = SEVEN[0];
    //Move_motor(current_face);
  }else if(current_face == 8){
    current_face = EIGHT[0];
    //Move_motor(current_face);
  }else if(current_face == 9){
    current_face = NINE[0];
    //Move_motor(current_face);
  }else if(current_face == 10){
    current_face = TEN[0];
    //Move_motor(current_face);
  }else if(current_face == 11){
    current_face = ELEVEN[0];
    //Move_motor(current_face);
  }else if(current_face == 12){
    current_face = TWELVE[0];
    //Move_motor(current_face);
  }
    Serial.println("Move DFA");/*
    digitalWrite(LED_THREE, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_THREE, LOW); */
}

void IFA(){
    if(current_face == 1){
    current_face = ONE[2];
    //Move_motor(current_face);
  }else if(current_face == 2){
    current_face = TWO[2];
    //Move_motor(current_face);
  }else if(current_face == 3){
    current_face = THREE[2];
    //Move_motor(current_face);
  }else if(current_face == 4){
    current_face = FOUR[2];
    //Move_motor(current_face);
  }else if(current_face == 5){
    current_face = FIVE[2];
    //Move_motor(current_face);
  }else if(current_face == 6){
    current_face = SIX[2];
    //Move_motor(current_face);
  }else if(current_face == 7){
    current_face = SEVEN[2];
    //Move_motor(current_face);
  }else if(current_face == 8){
    current_face = EIGHT[2];
    //Move_motor(current_face);
  }else if(current_face == 9){
    current_face = NINE[2];
    //Move_motor(current_face);
  }else if(current_face == 10){
    current_face = TEN[2];
    //Move_motor(current_face);
  }else if(current_face == 11){
    current_face = ELEVEN[2];
    //Move_motor(current_face);
  }else if(current_face == 12){
    current_face = TWELVE[2];
    //Move_motor(current_face);
  }
    Serial.println("Move IFA");/*
    digitalWrite(LED_FOUR, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_FOUR, LOW); */

}

void DFB(){
    if(current_face == 1){
    current_face = ONE[4];
    //Move_motor(current_face);
  }else if(current_face == 2){
    current_face = TWO[4];
    //Move_motor(current_face);
  }else if(current_face == 3){
    current_face = THREE[4];
    //Move_motor(current_face);
  }else if(current_face == 4){
    current_face = FOUR[4];
    //Move_motor(current_face);
  }else if(current_face == 5){
    current_face = FIVE[4];
    //Move_motor(current_face);
  }else if(current_face == 6){
    current_face = SIX[4];
    //Move_motor(current_face);
  }else if(current_face == 7){
    current_face = SEVEN[4];
    //Move_motor(current_face);
  }else if(current_face == 8){
    current_face = EIGHT[4];
    //Move_motor(current_face);
  }else if(current_face == 9){
    current_face = NINE[4];
    //Move_motor(current_face);
  }else if(current_face == 10){
    current_face = TEN[4];
    //Move_motor(current_face);
  }else if(current_face == 11){
    current_face = ELEVEN[4];
    //Move_motor(current_face);
  }else if(current_face == 12){
    current_face = TWELVE[4];
    //Move_motor(current_face);
  }
    Serial.println("Move DFB");/*
    digitalWrite(LED_FIVE, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_FIVE, LOW); */
}

void IFB(){
    if(current_face == 1){
    current_face = ONE[3];
    //Move_motor(current_face);
  }else if(current_face == 2){
    current_face = TWO[3];
    //Move_motor(current_face);
  }else if(current_face == 3){
    current_face = THREE[3];
    //Move_motor(current_face);
  }else if(current_face == 4){
    current_face = FOUR[3];
    //Move_motor(current_face);
  }else if(current_face == 5){
    current_face = FIVE[3];
    //Move_motor(current_face);
  }else if(current_face == 6){
    current_face = SIX[3];
    //Move_motor(current_face);
  }else if(current_face == 7){
    current_face = SEVEN[3];
    //Move_motor(current_face);
  }else if(current_face == 8){
    current_face = EIGHT[3];
    //Move_motor(current_face);
  }else if(current_face == 9){
    current_face = NINE[3];
    //Move_motor(current_face);
  }else if(current_face == 10){
    current_face = TEN[3];
    //Move_motor(current_face);
  }else if(current_face == 11){
    current_face = ELEVEN[3];
    //Move_motor(current_face);
  }else if(current_face == 12){
    current_face = TWELVE[3];
    //Move_motor(current_face);
  }
    Serial.println("Move IFB");/*
    digitalWrite(LED_SIX, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_SIX, LOW); */
}
//AF->IFA->AF
void A(){
  AF();
  IFA();
  AF();
    Serial.println("Move A");/*
    digitalWrite(LED_SEVEN, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_SEVEN, LOW); */
}
//AF->IFA
void DAA(){
  AF();
  IFA();
    Serial.println("Move DAA");/*
    digitalWrite(LED_EIGHT, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_EIGHT, LOW); */
}
//AF->DFA
void IAA(){
  AF();
  DFA();
    Serial.println("Move IAA");/*
    digitalWrite(LED_NINE, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_NINE, LOW); */
}
//AF->IFA->IFA
void DAB(){
  AF();
  IFA();
  IFA();
    Serial.println("Move DAB");/*
    digitalWrite(LED_TEN, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_TEN, LOW); */
}
//DFB->DFB
void IAB(){
  DFB();
  DFB();
    Serial.println("Move IAB");/*
    digitalWrite(LED_ELEVEN, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_ELEVEN, LOW); */
}
//DFB->AF
void AA(){
  DFB();
  AF();
    Serial.println("Move AA");/*
    digitalWrite(LED_TWELVE, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(1000);
     digitalWrite(LED_ONE, LOW); */
}

int receiveCommand() {
    int clientSt = 99;
    // this is our 'wait for incoming subscription packets' busy subloop
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(10))) {
        if(subscription == &tCommands){
            Serial.print(F("Got: "));
            Serial.println((char*)tCommands.lastread);
            
            publishClientStatus(false);

            char* commands = (char*)tCommands.lastread;
            int i = 0;
            std::string* commmand = new std::string(commands);
            const char * c = commmand->c_str();

            if(c[0] == '[') {
                Serial.println("-----BEGIN COMMANDS-----");
                i++;
                while (c[i - 1] != ']') {
                    if (c[i] == '\'') {
                      std::ostringstream os;
                      std::string str;
                        i++;
                        while (c[i] != '\'') {
                            os.put(c[i]);
                            i++;
                        }
                        str = os.str().c_str();
                        i++;
                        //Serial.print("COMMAND: ");
                        //Serial.println(str.c_str());
                        if (strcmp (str.c_str(),"AF") == 0) {
                            AF();
                        } else if (strcmp (str.c_str(),"F") == 0) {
                          F_MOVE();
                        }else if (strcmp (str.c_str(),"DFA") == 0) {
                          DFA();
                        }else if (strcmp (str.c_str(),"IFA") == 0) {
                          IFA();
                        }else if (strcmp (str.c_str(),"DFB") == 0) {
                          DFB();
                        }else if (strcmp (str.c_str(),"IFB") == 0) {
                          IFB();
                        }else if (strcmp (str.c_str(),"A") == 0) {
                          A();
                        }else if (strcmp (str.c_str(),"DAA") == 0) {
                          DAA();
                        }else if (strcmp (str.c_str(),"IAA") == 0) {
                          IAA();
                        }else if (strcmp (str.c_str(),"DAB") == 0) {
                          DAB();
                        }else if (strcmp (str.c_str(),"IAB") == 0) {
                          IAB();
                        }else if (strcmp (str.c_str(),"AA") == 0) {
                          AA();
                        }
                    }
                    i++;
                }
                Serial.print("Finish reading command");
            }
        }
    }

    //publishClientStatus(true);
    return clientSt;
}

