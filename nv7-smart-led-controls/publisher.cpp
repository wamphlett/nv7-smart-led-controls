#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include "controller.h"
#include "publisher.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.10.239";
int        port     = 1883;
const char topic[]  = "arduino/test";

Publisher::Publisher() {

}

void Publisher::PublishEvent(PublishEventType t) {
  StaticJsonDocument<200> doc;
  this->publishMessage(t, doc);
}

void Publisher::PublishButtonPushed(Button button, Channel channel) {
  StaticJsonDocument<200> doc;
  doc["button"] = stringifyButton(button);
  doc["channel"] = stringifyChannel(channel);
  this->publishMessage(BUTTON_PRESS, doc);
}

void Publisher::publishMessage(PublishEventType type, StaticJsonDocument<200> doc) {
  if (!mqttClient.connected()) {
    Serial.println("connecting to broker");
    if (!mqttClient.connect(broker, port)) {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.connectError());

      return;
    }
  }

  doc["type"] = stringifyPublishEventType(type);
  
  String jsonString;
  serializeJson(doc, jsonString);

  Serial.println(jsonString);

  mqttClient.beginMessage(topic);
  mqttClient.print(jsonString);
  mqttClient.endMessage();
}

String stringifyPublishEventType(PublishEventType t) {
  switch (t) {
    case STARTUP:
      return "STARTUP";
    case BUTTON_PRESS:
      return "BUTTON_PRESS";
  }
  return "";
}