#include <Arduino.h>
#include <ArduinoJson.h>
#include "controller.h"

#ifndef PUBLISHER_H
#define PUBLISHER_H

enum PublishEventType {
  STARTUP,
  BUTTON_PRESS
};

String stringifyPublishEventType(PublishEventType);

class Publisher {
  public: 
    Publisher(); // constructor
    void PublishEvent(PublishEventType);
    void PublishButtonPushed(Button, Channel);
  
  private:
    void publishMessage(PublishEventType, StaticJsonDocument<200>); // publishes messages to the "nv7/events" topic
};

#endif // PUBLISHER_H