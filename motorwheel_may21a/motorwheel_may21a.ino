const int switchPin = 2;
const int motorPin = 10;
void setup() {
    pinMode(switchPin, INPUT);
    pinMode(motorPin, OUTPUT);
}

void loop() {
    if(digitalRead(switchPin) == HIGH) {
        digitalWrite(motorPin, HIGH);
    } else {
        digitalWrite(motorPin, LOW);
    }
}
