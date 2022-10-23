#include <SPI.h>
#include <WiFiNINA.h>
#include "secret.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;
boolean alreadyConnected = false;

WiFiServer server(23);

void setup() {
  Serial.begin(9600);
  while (status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    if (!alreadyConnected) {
      client.flush();
      Serial.print("Connected to: ");
      Serial.println(client.remoteIP());
      alreadyConnected = true;
      }
    while (client.available() > 0) {
      char thisChar = client.read();
      Serial.write(thisChar);
      }
    }
  while (Serial.available() > 0) {
    char c = Serial.read();
    client.print(c);
    Serial.write(c);
    Serial.print(client);
  }
  alreadyConnected = false;
}
