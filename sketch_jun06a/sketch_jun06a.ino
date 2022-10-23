void setup() {
  pinMode(34, INPUT_PULLUP);
  pinMode(31, OUTPUT);
}

void loop() {
  digitalWrite(31, digitalRead(34));
}
