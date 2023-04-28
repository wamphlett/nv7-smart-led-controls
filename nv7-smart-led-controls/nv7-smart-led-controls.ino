int ledPin = 3;
int readPin = 14;
int dReadPin = 2;

enum Channel {
  A, 
  B
};

enum Button {
  UNKNOWN,
  CHANGE_CHANNEL,
  MODE,
  COLOR,
  SPEED
};

struct State {
  Button lastButton;
  Channel currentChannel;
};

State controllerState = {UNKNOWN, A};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting!");
  pinMode(readPin, INPUT_PULLUP);
  pinMode(dReadPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // set the current channel
  setChannel(A);
}

void loop() {
  // connectingAnimation();
  // return;

  // put your main code here, to run repeatedly:
  int pinState = analogRead(readPin); // Read the state of the pin

  if (isIdle(pinState)) {
    controllerState.lastButton = UNKNOWN;
    return;
  }

  // determine what button was pushed
  Button currentButton = getButton(pinState);

  // Dont do anything if the current button has not changed
  if (controllerState.lastButton == currentButton) {
    return;
  }

  controllerState.lastButton = currentButton;

  switch (currentButton) {
    case CHANGE_CHANNEL:
      toggleChannel();
      break;
    case MODE:
      Serial.println("MODE");
      break;
    case SPEED:
      Serial.println("SPEED");
      break;
    case COLOR:
      Serial.println("COLOR");
      break;
  }

  delay(10);
}

bool isIdle(int state) {
  return state > 500;
}

bool isTarget(int state, int target) {
  return state < target + 2 && state > target - 2;
}

Button getButton(int pinState) {
  if (isTarget(pinState, 243)) {
    return CHANGE_CHANNEL;
  }

  if (isTarget(pinState, 14)) {
    return COLOR;
  }

  if (isTarget(pinState, 66)) {
    return SPEED;
  }

  if (isTarget(pinState, 127)) {
    return MODE;
  }

  return UNKNOWN;
}

void setChannel(Channel c) {
  if (c == A) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  controllerState.currentChannel = c;
}

void toggleChannel() {
  if (controllerState.currentChannel == A) {
    setChannel(B);
  } else {
    setChannel(A);
  }
}

void connectingAnimation() { 
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(80);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(800);
}