const int greenLED = 9;
const int blueLED = 10;
const int redLED = 11;
const int redSensor = A0;
const int greenSensor = A1;
const int blueSensor = A2;
int redLEDBright;
int greenLEDBright;
int blueLEDBright;

void setup() {
  Serial.begin(19200);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);  
}

void loop() {
  int redValue = analogRead(redSensor);
  int greenValue = analogRead(greenSensor);
  int blueValue = analogRead(blueSensor);
  Serial.print("Raw value:");
  Serial.print("R: ");
  Serial.print(redValue);
  Serial.print("G: ");
  Serial.print(greenValue);
  Serial.print("B: ");
  Serial.println(blueValue);
  redValue = redValue / 4;
  greenValue = greenValue / 4;
  blueValue = blueValue / 4;
  Serial.print("LED Value: ");
  Serial.print("R: ");
  Serial.print(redValue);
  Serial.print("G: ");
  Serial.print(greenValue);
  Serial.print("B: ");
  Serial.println(blueValue);
  analogWrite(redLED, redValue);
  analogWrite(greenLED, greenValue);
  analogWrite(blueLED, blueValue);
  delay(100);
}
