#include "controller.h"
#include <Arduino.h>

Controller::Controller(int buttonPin, int ledPin, int pollRate) 
  : _buttonPin(buttonPin), _ledPin(ledPin), _pollRate(pollRate) {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // default the channel to A
  this->setChannel(A);
}

void Controller::Poll() {
  if ((millis() - this->_lastPoll) < this->_pollRate) {
    return;
  }
  this->_lastPoll = millis();

  this->_buttonState = analogRead(this->_buttonPin);

  // return early if there is no button being pressed
  if (this->IsIdle()) {
    this->_currentButton = NONE;
    this->_lastPress = millis();
    return;
  }

  // record the previous button
  this->_previousButton = this->_currentButton;

  // record the current button
  this->_currentButton = this->getButton();

  // if the current button does not match the previous button, the button was pressed
  if (this->_previousButton != this->_currentButton) {
    // only update the last press if the button was not held
    if (this->_currentButton != MODE_HOLD) {
      this->_lastPress = millis();
    }
    this->_onPressFunc(this->_currentButton);
  }
}

void Controller::OnPress(void (*_onPressFunc)(Button)) {
  this->_onPressFunc = _onPressFunc;
}

Channel Controller::CurrentChannel() {
  return this->_currentChannel;
}

bool Controller::IsIdle() {
  return this->_buttonState > 500;
}

Button Controller::getButton() {
  if (this->isTarget(this->_buttonState, 243)) {
    return CHANGE_CHANNEL;
  }

  if (this->isTarget(this->_buttonState, 14)) {
    return COLOR;
  }

  if (this->isTarget(this->_buttonState, 66)) {
    return SPEED;
  }

  if (this->isTarget(this->_buttonState, 127)) {
    // mode is able to be held, check if the button is being held
    if ((millis() - this->_lastPress) > 800) {
      return MODE_HOLD;
    }
    return MODE;
  }

  return NONE;
}

bool Controller::isTarget(int state, int target) {
  return state < target + 2 && state > target - 2;
}

void Controller::ToggleChannel() {
  if (this->_currentChannel == A) {
    this->setChannel(B);
  } else {
    this->setChannel(A);
  }
}

void Controller::setChannel(Channel c) {
  if (c == A) {
    digitalWrite(this->_ledPin, HIGH);
  } else {
    digitalWrite(this->_ledPin, LOW);
  }
  this->_currentChannel = c;
}

String stringifyButton(Button button) {
  switch (button) {
    case NONE:
      return "NONE";
    case CHANGE_CHANNEL:
      return "CHANGE_CHANNEL";
    case MODE:
      return "MODE";
    case MODE_HOLD:
      return "MODE_HOLD";
    case COLOR:
      return "COLOR";
    case SPEED:
      return "SPEED";
    default:
      return "";
  }
}

String stringifyChannel(Channel channel) {
  switch (channel) {
    case A:
      return "A";
    case B:
      return "B";
    default:
      return "";
  }
}

