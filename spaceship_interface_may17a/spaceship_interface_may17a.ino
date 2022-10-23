int switchState;
void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  switchState = digitalRead(2);
  // read switch state
  if(switchState == LOW) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else {
    int i = 0;
    // flashes 10 times
    while(i < 10) {
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      delay(100);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      delay(100);
      i = i + 1;
    }
  }
}
