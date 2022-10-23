#include <CapacitiveSensor.h>
CapacitiveSensor capSensor = CapacitiveSensor(4, 2);
int threshold = 1000;
boolean lightOn = false;
const int ledPin = 12;
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  long sensorValue = capSensor.capacitiveSensor(30);
  Serial.println(sensorValue);
  if(sensorValue > threshold) {
    lightOn = !lightOn;
    delay(500);
  }

 if(lightOn == false) {
  digitalWrite(ledPin, LOW);
 } else {
  digitalWrite(ledPin, HIGH);
 }
  delay(10);
}
