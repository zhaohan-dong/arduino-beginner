/* 
 Sketch generated by the Arduino IoT Cloud Thing "env_shield"
 https://create.arduino.cc/cloud/things/829941ed-efdd-4572-91c0-e93a732192ec 
 Arduino IoT Cloud Properties description
 The following variables are automatically generated and updated when changes are made to the Thing properties
 int humidity;
 int lux;
 int pressure;
 int temperature;
 int uva;
 int uvb;
 int uvi;
 Properties which are marked as READ/WRITE in the Cloud Thing will also have functions
 which are called when their values are changed from the Dashboard.
 These functions are generated with the Thing and added at the end of this sketch.
*/
#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Arduino_MKRENV.h>
void setup() {
 // Initialize serial and wait for port to open:
 Serial.begin(9600);
 // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
 delay(1500); 
 // Defined in thingProperties.h
 initProperties();
 // Connect to Arduino IoT Cloud
 ArduinoCloud.begin(ArduinoIoTPreferredConnection);
 /*
    The following function allows you to obtain more information
    related to the state of network and IoT Cloud connection and errors
    the higher number the more granular information you’ll get.
    The default is 0 (only errors).
    Maximum is 4
*/
 setDebugMessageLevel(4);
 ArduinoCloud.printDebugInfo();
 if (!ENV.begin()) {
   Serial.println("Failed to initialize MKR ENV shield!");
   while(1);
 }
}
void loop() {
 ArduinoCloud.update();
 // Your code here 
 humidity = int(ENV.readHumidity());
 lux = int(ENV.readIlluminance());
 pressure = int(ENV.readPressure());
 temperature = int(ENV.readTemperature());
 uva = int(ENV.readUVA());
 uvb = int(ENV.readUVB());
 uvi = int(ENV.readUVIndex());
 delay(1000);
}
void onHumidityChange() {
 // Do something
}
void onLuxChange() {
 // Do something
}
void onPressureChange() {
 // Do something
}
void onTemperatureChange() {
 // Do something
}
void onUvaChange() {
 // Do something
}
void onUvbChange() {
 // Do something
}
void onUviChange() {
 // Do something
}
