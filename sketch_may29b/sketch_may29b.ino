void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  int sig = digitalRead(12);
  Serial.print(sig);
}
