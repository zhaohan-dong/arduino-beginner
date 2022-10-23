const int switchPin = 12;
int switchState = 0;
int previousSwitchState = 0;
unsigned long previousTime = 0;
int ledMin = 1;
int ledFiveMin = 6;
int ledTenMin = 7;
long minute = 60000;
int minutes = 1;

void setup() {
  for(int x = 2;x<10;x++) {
    pinMode(x, OUTPUT);
  }
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

  // When time is at one minute
  if(currentTime - previousTime >= minute) {
    ledMin++;
    previousTime = currentTime;
    digitalWrite(ledMin, HIGH);
    Serial.print("Minute: ");
    Serial.println(minutes);
    Serial.println(currentTime);
    minutes++;
  }

  // When all four one-minute LEDs light up and five-minute LED is not lit
  if(ledMin == 6 && ledFiveMin == 6) {
    digitalWrite(ledFiveMin, HIGH);
    for(int x = 2;x<6;x++) {
      digitalWrite(x, LOW);
    }
    ledMin = 1;
    ledFiveMin++;
  } else if(ledMin == 6 && ledFiveMin == 7) {
    // When at 10 minute mark
    digitalWrite(ledTenMin, HIGH);
    for(int x = 2; x<7;x++) {
      digitalWrite(x, LOW);
    }
    ledMin = 1;
    ledFiveMin = 6;
    ledTenMin++;
  }
  
  if(ledTenMin == 10) { //When all the lights are on
    for(int x = 2;x<10;x++) {
      digitalWrite(x, LOW);
    }
    ledMin = 1;
    ledFiveMin = 6;
    ledTenMin = 7;
  }
  // Reset when switch has changed
  switchState = digitalRead(switchPin);
  if(switchState != previousSwitchState) {
    for(int x = 2;x< 10;x++) {
      digitalWrite(x, LOW);
    }
    ledMin = 1;
    ledFiveMin = 6;
    ledTenMin = 7;
    previousSwitchState = switchState;
  }
}
