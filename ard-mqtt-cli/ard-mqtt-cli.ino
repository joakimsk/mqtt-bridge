// Based on code by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <Ethernet.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include "secret.h"

// Addressing
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 32, 11}; // Device IP
//byte mqttserver[] = {54, 76, 137, 235};
const char* mqttserver = "m23.cloudmqtt.com";

//const char* mqttusername = "secret"; // Defined in secret.h
//const char* mqttpassword = "secret"; // Defined in secret.h

int port = 10010;

// Objects
EthernetClient net;
MQTTClient client;

// Housekeeping
unsigned long lastMillis = 0;
DynamicJsonDocument doc(32);
char messagebuffer[32];

int ledPin = 13;  // LED connected to digital pin 13
int inPin = 7;    // pushbutton connected to digital pin 7
int val = 0;      // variable to store the read value

void connect() {
  Serial.print("connecting...");
  while (!client.connect("arduino", mqttusername, mqttpassword))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("statsnail/steinvikveien/ras");
  // client.unsubscribe("/hello");
}

/*void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}*/

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  client.begin(mqttserver, port, net);
  //client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();

  if (!client.connected())
  {
    connect();
  }

  // publish a message roughly every 30 second.
  if (millis() - lastMillis > 30000)
  {
    lastMillis = millis();

    val = digitalRead(inPin);   // read the input pin
    digitalWrite(ledPin, val);  // sets the LED to the button's value

    doc["sensor"] = val;
    size_t n = serializeJson(doc, messagebuffer);
    
    client.publish("statsnail/steinvikveien/room", messagebuffer);
  }
}
