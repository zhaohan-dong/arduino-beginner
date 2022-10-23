#include <LiquidCrystal.h>
LiquidCrystal lcd(30, 28, 26, 24, 22, 23, 25, 27, 29, 31);
const int switchPin = 52;
const int backlightPin = 53;
int backlightState = 1;
byte start;
String timestamp;
String temperature;
String dewPoint;
String qNH;
String altimeter;
unsigned long timer;
String prevLine1;
String prevLine2;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(switchPin, INPUT);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, HIGH);
  lcd.begin(16, 2);
  digitalWrite(backlightPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(switchPin), backlight, RISING);
}

void backlight() {
  backlightState = !backlightState;
  digitalWrite(backlightPin, backlightState);
}

void disp(String line1, String line2) {
  if(prevLine1 != line1 || prevLine2 != line2){
    prevLine1 = line1;
    prevLine2 = line2;
    lcd.clear();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}

void loop() {
  if(Serial1.available()) {
    start = Serial1.read();
    if(start == 36) {
      timestamp = Serial1.readStringUntil(' ');
      temperature = Serial1.readStringUntil('/');
      dewPoint = Serial1.readStringUntil(' ');
      qNH = Serial1.readStringUntil('/');
      altimeter = Serial1.readStringUntil(' ');
      Serial.print("Time: ");
      Serial.println(timestamp);
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Dew Point: ");
      Serial.println(dewPoint);
      Serial.print("QNH: ");
      Serial.println(qNH);
      Serial.print("Altimeter: ");
      Serial.println(altimeter);
      }
  }
  if (millis() - timer < 2000) {
    disp("Time", timestamp);
  } else if (millis() - timer < 4000) {
    disp("Temperature", temperature);
  } else if (millis() - timer < 6000) {
    disp("Dew Point", dewPoint);
  } else if (millis() - timer < 8000) {
    disp("QNH", qNH);
  } else if (millis() - timer < 10000) {
    disp("Altimeter", altimeter);
  } else {
    timer = millis();
  }
}
