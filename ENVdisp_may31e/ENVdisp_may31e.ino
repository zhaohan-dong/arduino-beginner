#include <Arduino_MKRENV.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(30, 28, 26, 24, 22, 23, 25, 27, 29, 31);
const int switchPin = 52;
const int backlightPin = 53;
int backlightState = 1;
double fieldElev = 30;
float temperature;
float humidity;
float dewPoint;
float pressure;
float illuminance;
float uVIndex;
float uva;
float uvb;
float qnh_raw;
float alt;
String qNH;


void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  pinMode(backlightPin, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(backlightPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(switchPin), backlight, RISING);
}

void backlight() {
  backlightState = !backlightState;
  digitalWrite(backlightPin, backlightState);
}

void disp(String line1, String line2) {
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

float dPoint(float temperature, float relativeHumidity) {
  //float a = 6.112;
  float b = 17.67;
  float c = 243.5;
  float g = log(relativeHumidity / 100) + (b * temperature / (c + temperature));
  return c * g / (b - g);
 }

void loop() {
  temperature = ENV.readTemperature();
  disp("Temperature", String(temperature));
  Serial.print(temperature);
  delay(5000);
  humidity = ENV.readHumidity();
  dewPoint = dPoint(temperature, humidity);
  disp("Dew Point", String(dewPoint));
  Serial.print(dewPoint);
  delay(5000);
  pressure = ENV.readPressure(MILLIBAR);
  disp("Pressure", String(pressure));
  Serial.print(pressure);
  delay(5000);
  illuminance = ENV.readIlluminance();
  uVIndex = ENV.readUVIndex();
  uva = ENV.readUVA();
  uvb = ENV.readUVB();
  if(uVIndex < 0) {
    uVIndex = 0;
  }
  if(uva < 0) {
    uva = 0;
  }
  if(uvb < 0) {
    uvb = 0;
  }
  qnh_raw = (pressure - 0.3) * pow(1 + 0.00002567294033 * fieldElev / pow(pressure - 0.3, 0.190284), 1 / 0.190284);
  alt = 2.95300 * qnh_raw;
  if (qnh_raw < 1000) {
    qNH = "Q0" + String(round(qnh_raw)) + "/A" + String(round(alt));
  } else {
    qNH = "Q" + String(round(qnh_raw)) + "/A" + String(round(alt)); 
  }
  disp("QNH/Altimeter", qNH);
  Serial.print(qNH);
  delay(5000);
}
