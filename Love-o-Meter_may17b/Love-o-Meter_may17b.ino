const int sensorPin = A0;
const float basetemp = 23;
void setup() {
  Serial.begin(9600);
  for(int pinNumber = 2; pinNumber < 5; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}
const float baselineTemp = ((analogRead(sensorPin) / 1024.0) * 5.0 - 0.5) * 100.0;
void loop() {
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor value");
  Serial.print(sensorVal);
  float voltage = (sensorVal / 1024.0) * 5.0;
  Serial.print("Voltage: ");
  Serial.print(voltage);
  float temp = (voltage - 0.5) * 100;
  Serial.print("Temperature: ");
  Serial.println(temp);
  if(basetemp< temp && temp < basetemp + 2) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else if(basetemp + 2 <= temp && temp < basetemp + 4) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  } else if (basetemp + 4 <= temp) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  delay(1);
}
