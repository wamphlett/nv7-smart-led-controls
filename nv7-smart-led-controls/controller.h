#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

// Defines a list of available channels
enum Channel {
  A, 
  B
};

// Defines a list of available buttons 
enum Button {
  NONE,
  CHANGE_CHANNEL,
  MODE,
  MODE_HOLD,
  COLOR,
  SPEED
};

String stringifyButton(Button);
String stringifyChannel(Channel);

// 
class Controller {
  public:
    Controller(int, int, int);
    void Poll();
    bool IsIdle();
    void OnPress(void (*func_ptr)(Button));
    Channel CurrentChannel();
    void ToggleChannel();
  
  private:
    int _ledPin; // the digital pin controlling the channel LEDs
    int _buttonPin; // the analog pin used to determine which button is being pressed
    int _pollRate; // how often to poll the hardware

    void (*_onPressFunc)(Button); // the function to call each time a button is pressed
  
    int _buttonState; // the current button state as read from the analog button pin
    unsigned long _lastPress; // millis representing the last time a button was pressed
    unsigned long _lastPoll; // millis representing the last time the controller was polled
    Button _currentButton; // the button determined on the most recent poll
    Button _previousButton; // the button determined on the previous poll
    Channel _currentChannel; // the channel determined on the most recent poll

    void setChannel(Channel);
    bool isTarget(int, int);
    int pinState();
    Button getButton();
};

#endif // CONTROLLER_H