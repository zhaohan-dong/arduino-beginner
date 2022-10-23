#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <Servo.h>
#include "Secret.h"
#include <TelegramBot.h>

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char BotToken[] = SECRET_BOT_TOKEN;
const char BotName[] = SECRET_BOT_NAME;
const char BotUsername[] = SECRET_BOT_USERNAME;
WiFiSSLClient client;
TelegramBot bot(BotToken, client);
TelegramKeyboard keyboard_one;
const char* row_one[] = {"Music and Food", "Music No Food"};
const char* row_two[] = {"One Portion", "Two Portion", "Three Portion"};


String oldChatId = "";

const int piezoPin = 6;
const int motorPin = 7;
int servoPos = 0;

bool startDetecting = false;
unsigned long timer;
Servo myServo;
int portionAmount;


int threshold = 300;
message m = bot.getUpdates();

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.print("connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  myServo.attach(motorPin);
  pinMode(piezoPin, OUTPUT);
  
  bot.begin();
  keyboard_one.addRow(row_one, 2);
  keyboard_one.addRow(row_two, 3);
  bot.sendMessage(m.chat_id, "Hello !", keyboard_one);
  
}

void dispense() {
  int pos;
  Serial.print("moving servo");
  for(pos = 0; pos <= 90; pos++) {
    myServo.write(pos);
    delay(15);
  }
  delay(portionAmount * 300);
  for(pos = 90; pos >= 0; pos-=1) {
    myServo.write(pos);
    delay(15);
  }
}

void loop() {
  if (m.chat_id != 0) {
    oldChatId = m.chat_id;
    Serial.println(m.text);
    if(m.text == "Music and Food") {
      bot.sendMessage(m.chat_id, "Dispensing "+String(portionAmount)+" of food right now", keyboard_one);
      tone(piezoPin, 440, 200);
      startDetecting = true;
      timer = millis();
      dispense();
      }
      
      if(m.text == "Music no Food") {
        bot.sendMessage(m.chat_id, "At your command", keyboard_one);
        startDetecting = true;
        timer = millis();
      }

      if(m.text == "One Portion") {
        portionAmount = 1;
        bot.sendMessage(m.chat_id, "Portion: One", keyboard_one);
      }

      if(m.text == "Two Portion") {
        portionAmount = 2;
        bot.sendMessage(m.chat_id, "Portion: Two", keyboard_one);
      }
        
      if(m.text == "Three Portion") {
        portionAmount = 3;
        bot.sendMessage(m.chat_id, "Portion: Three", keyboard_one);
      }
  }

  if(startDetecting == true) {
    int light = analogRead(A1);
    if (light < threshold) {
        String timeValue = String((millis() - timer) / 1000);
        bot.sendMessage(oldChatId, "Cat detected in "+timeValue);
        startDetecting = false;
      } else if(millis() - timer > 120000) {
        bot.sendMessage(oldChatId, "No cat detected in two minutes");
        startDetecting = false;
      }
  }
  delay(1000);
}
