#include <ArduinoBLE.h>
BLEService relay("1101");
BLEBooleanCharacteristic relayState("1201", BLERead | BLEWrite);

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  BLE.begin();
  BLE.setLocalName("Relay");
  BLE.setAdvertisedService("Relay");
  relay.addCharacteristic(relayState);
  BLE.addService(relay);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  while (central.connected()) {
    if (relayState.written()) {
      Serial.println(relayState.value());
      digitalWrite(2, relayState.value());
    }
  }

}
