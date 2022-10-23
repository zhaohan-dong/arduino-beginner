#include <Arduino_MKRENV.h>
#include <math.h>
int fieldElev;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Enter station elevation(ft): ");
  while(Serial.available() < 1);
  fieldElev = Serial.parseInt();
  Serial.println(fieldElev);
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
}

float dewPoint (float temperature, float relativeHumidity) {
  float a = 6.112;
  float b = 17.67;
  float c = 243.5;
  float g = log(relativeHumidity / 100) + (b * temperature / (c + temperature));
  return c * g / (b - g);
}

void loop() {
  float temperature = ENV.readTemperature();
  float humidity = ENV.readHumidity();
  float dPoint = dewPoint(temperature, humidity);
  float pressure = ENV.readPressure(MILLIBAR);
  float illuminance = ENV.readIlluminance();
  float uva = ENV.readUVA();
  float uvb = ENV.readUVB();
  float uvIndex = ENV.readUVIndex();
  float pA = 145366.45 * (1 - pow(pressure / 1013.25, 0.190284));
  float qnh = (pressure - 0.3) * pow(1 + 0.000084228806861403 * 0.3048 * fieldElev / pow(pressure - 0.3, 0.190284), 1 / 0.190284);
  float alt = 2.95300 * qnh;
  float vaporPressure = 6.11 * pow(10, 7.5 * dPoint / (237.7 + dPoint));
  int dA = 100 * round(1453.6645 * (1 - pow(0.28424265555556 * pressure / (273.16 + temperature) * (1 - vaporPressure / pressure * 0.378), 0.235)));
  String strTemp;
  String strDPnt;
  
  if (temperature >= 0 && temperature < 10) { // format temperature display
    strTemp = "0" + String(round(temperature));
  } else if (temperature > 10) {
    strTemp = String(round(temperature));
  } else if (temperature < 0 && temperature > -10) {
    strTemp = "M0" + String(round(abs(temperature)));
  } else if (temperature <= -10) {
    strTemp = "M" + String(round(abs(temperature)));
  }
  
  if (dPoint >= 0 && dPoint < 10) { // format dew point display
    strDPnt = "0" + String(round(dPoint));
  } else if (dPoint > 10) {
    strDPnt = String(round(dPoint));
  } else if (dPoint < 0 && dPoint > -10) {
    strDPnt = "M0" + String(round(abs(dPoint)));
  } else if (dPoint <= -10) {
    strDPnt = "M" + String(round(abs(dPoint)));
  }
  
  Serial.print(strTemp);
  Serial.print("/");
  Serial.print(strDPnt);
  Serial.print(" ");

  Serial.print("Q");
  Serial.print(round(qnh));
  Serial.print("/");
  Serial.print("A");
  Serial.println(round(alt));

  Serial.print("Station Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Pressure Altitude = ");
  Serial.print(round(pA));
  Serial.println(" ft");

  Serial.print("Density Altitude = ");
  Serial.print(dA);
  Serial.println(" ft");  

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA = ");
  Serial.println(uva);

  Serial.print("UVB = ");
  Serial.println(uvb);
  
  Serial.print("UV Index = ");
  Serial.println(uvIndex);

  Serial.println();
  delay(10000);
}
