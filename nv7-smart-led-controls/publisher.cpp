#include <ArduinoJson.h>
#include "controller.h"
#include "publisher.h"

Publisher::Publisher() {}

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
  doc["type"] = stringifyPublishEventType(type);
  
  String jsonString;
  serializeJson(doc, jsonString);

  Serial.println(jsonString);
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