int freq = 20;
void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}

void loop() {
  digitalWrite(2, LOW);
  delay(5);
  digitalWrite(2, HIGH);
  delay(5);
}
