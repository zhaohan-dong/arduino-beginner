const int switchPin = 13;
const int motorPin = 3;
const int potPin = A0;
int switchPinState = 0;
int speedLow = 1023;
int speedHigh = 0;
int calibrated = 0;
int previousSwitchState = 0;
void setup() {
  pinMode(switchPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(potPin, INPUT);
  while(calibrated = 0) { // when button not pressed for calibration
    if(analogRead(potPin) < speedLow) {
      speedLow = analogRead(potPin);
    } else if(analogRead(potPin) > speedHigh) {
      speedHigh = analogRead(potPin);
    }
    if(digitalRead(switchPin) == HIGH) {
      calibrated = 1;
      delay(200);
    }
  }
}

void loop() {
  int speed = map(analogRead(potPin), speedLow, speedHigh ,0 ,255);
  
  // when button pressed turn on/off motor
  if(digitalRead(switchPin) != previousSwitchState) {
    if(digitalRead(switchPin) == HIGH) {
      switchPinState = !switchPinState;
      }
   }
  if(switchPinState == 1 && speed > 10) { // when speed is more than 10
      analogWrite(motorPin, speed);
  } else {
    analogWrite(motorPin, 0);
  }
  previousSwitchState = digitalRead(switchPin);
}
