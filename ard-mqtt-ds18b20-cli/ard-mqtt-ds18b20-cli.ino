// Put together by Joakim Skjefstad
// Based on code by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt
// and Konstantin Dimitrov
// https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806

#include <Ethernet.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include "secret.h"

// DS18B20 one-wire communication
#define ONE_WIRE_BUS 2 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

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

int val = 0;      // variable to store the read value

void connect() {
  Serial.print("connecting...");
  while (!client.connect("arduino", mqttusername, mqttpassword))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}



void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  client.begin(mqttserver, port, net);
  //client.onMessage(messageReceived);
  sensors.begin();
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

    sensors.requestTemperatures();
    val = sensors.getTempCByIndex(0);
    Serial.println(val);

    doc["temperature"] = val;
    size_t n = serializeJson(doc, messagebuffer);
    
    client.publish("statsnail/steinvikveien/room", messagebuffer);
  }
}
