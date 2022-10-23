#include <ArduinoLowPower.h>
#include <WiFiNINA.h>
#include <RTCZero.h>
#include <tone.h>
#include "secret.h"

RTCZero rtc;
const byte interruptPin = 0;
const byte piezoPin = 2;
const byte led1Pin = 3;
const byte led2Pin = 4;
const byte led3Pin = 5;
const byte led4Pin = 6;
const byte led5Pin = 7;
boolean countdown = false;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  pinMode(led5Pin, OUTPUT);
  rtc.begin();
  // interrupt on pin
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(interruptPin), sync, FALLING);
  
  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  unsigned long epoch;
  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
    }
  while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
    }
  else {
    rtc.setEpoch(epoch);
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    Serial.println();
    }
}

void loop() {
  LowPower.deepSleep();
}

// sync mode
void sync() {
  Serial.begin(9600);
  Serial.println("break");
  while (rtc.getSeconds() != 55) {
    countdown = true;
  }
  while (countdown) {
    Serial.println("counting");
    Serial.println(rtc.getSeconds());
    if (rtc.getSeconds() == 56) {
      digitalWrite(led1Pin, HIGH);
      tone(piezoPin, 440, 100);
    }
    if (rtc.getSeconds() == 57) {
      digitalWrite(led2Pin, HIGH);
      tone(piezoPin, 440, 100);
    }
    if (rtc.getSeconds() == 58) {
      digitalWrite(led3Pin, HIGH);
      tone(piezoPin, 440, 100);
    }
    if (rtc.getSeconds() == 59) {
      digitalWrite(led4Pin, HIGH);
      tone(piezoPin, 440, 50);
    }
    if (rtc.getSeconds() == 0) {
      digitalWrite(led5Pin, HIGH);
      tone(piezoPin, 587, 100);
    }
    if (rtc.getSeconds() == 1) {
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);
      digitalWrite(led4Pin, LOW);
      digitalWrite(led5Pin, LOW);
      noTone(piezoPin);
      countdown = false;
    }
  }
}
