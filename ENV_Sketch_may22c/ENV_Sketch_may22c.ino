#include <Arduino_MKRENV.h>
void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
}

void loop() {
  float temperature = ENV.readTemperature();
  float humidity = ENV.readHumidity();
  float pressure = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva = ENV.readUVA();
  float uvb = ENV.readUVB();
  float uvIndex = ENV.readUVIndex();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Station Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

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
  delay(60000);
}
