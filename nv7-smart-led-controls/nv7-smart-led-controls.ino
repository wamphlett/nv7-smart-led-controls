#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "config.h"
#include "controller.h"
#include "publisher.h"

#include <utility/wifi_drv.h>
const int greenPin = 25;
const int redPin = 26;
const int bluePin = 27;

Controller controller = Controller(BUTTON_PIN, LED_PIN, 10);
Publisher publisher = Publisher();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  statusWarn();

  WiFi.begin(WIFI_SECRET_SSID, WIFI_SECRET_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    connectingAnimation(1000);
  }
  Serial.println("Connected to WiFi");
  statusOkay();

  // register the function to be executed when a button is pressed
  controller.OnPress(handlePress);
  // send the STARTUP event 
  publisher.PublishEvent(STARTUP);
}

void loop() {
  // connectingAnimation();
  // return;

  // poll the controller every 10ms
  controller.Poll();
}

void connectingAnimation(int duration) {
  unsigned long start = millis();
  while ((millis() - start) < duration) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(80);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(800);
  }
}

// function to be executed each time a button is pressed
void handlePress(Button button) {
  if (button == CHANGE_CHANNEL) {
    controller.ToggleChannel();
    if (controller.CurrentChannel() == A) {
      Serial.println("Channel set to A");
    } else {
      Serial.println("Channel set to B");
    }
  }

  publisher.PublishButtonPushed(button, controller.CurrentChannel());
}

void statusWarn() {
  WiFiDrv::analogWrite(redPin, 255);
  WiFiDrv::analogWrite(greenPin, 0);
  WiFiDrv::analogWrite(bluePin, 0);
}

void statusOkay() {
  WiFiDrv::analogWrite(redPin, 0);
  WiFiDrv::analogWrite(greenPin, 255);
  WiFiDrv::analogWrite(bluePin, 0);
}
