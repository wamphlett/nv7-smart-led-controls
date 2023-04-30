#include "config.h"
#include "controller.h"
#include "publisher.h"

Controller controller = Controller(BUTTON_PIN, LED_PIN, 10);
Publisher publisher = Publisher();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

void connectingAnimation() { 
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(80);
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(800);
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