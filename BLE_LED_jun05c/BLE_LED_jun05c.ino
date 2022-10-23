#include <ArduinoBLE.h>
BLEService LEDLights("1101");
BLEByteCharacteristic LEDGreen("2101", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic LEDYellow("2102", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic LEDRed("2103", BLERead | BLEWrite | BLENotify);

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  BLE.begin();
  Serial.begin(9600);
  BLE.setLocalName("LEDs");
  BLE.setAdvertisedService("LEDs");
  LEDLights.addCharacteristic(LEDGreen);
  LEDLights.addCharacteristic(LEDYellow);
  LEDLights.addCharacteristic(LEDRed);
  BLE.addService(LEDLights);
  BLE.advertise();
  Serial.println("Starting Bluetooth...");
}

void loop() {
  BLEDevice central = BLE.central();
  while (central.connected()) {
    if (LEDGreen.written()) {
      Serial.print("Green Level");
      Serial.println(LEDGreen.value());
      analogWrite(2, LEDGreen.value());
    }
    if (LEDYellow.written()) {
      Serial.print("Yellow Level");
      Serial.println(LEDYellow.value());
      analogWrite(3, LEDYellow.value());
    }
    if (LEDRed.written()) {
      Serial.print("Red Level");
      Serial.println(LEDRed.value());
      analogWrite(5, LEDRed.value());
    }
  }
}
