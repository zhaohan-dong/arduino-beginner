#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int lastStationPin = 10;
const int nextStationPin = 9;
const int callStationPin = 6;
int stationNumber = 0;
int prevLastStation;
int prevNextStation;
int prevCallStation;
boolean callingStation = true;
String trainNumber = "RB 2";
String dir = "Mannheim Hbf";
String station[] = {"Karlsruhe Hbf", "Karlsruhe-Hagsfeld", "Blankenloch", "Friedrichstal(Baden)",
                      "Graben-Neudorf", "Wiesental", "Waghäusel", "Neulußheim", "Hockenheim",
                      "Oftersheim", "Schwetzingen", "Mannheim-Rheinau", "Mannheim-Neckarau", "Mannheim Hbf"};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(lastStationPin, INPUT);
  pinMode(nextStationPin, INPUT);
  pinMode(callStationPin, INPUT);
  lcd.clear();
  lcd.noAutoscroll();
}

int printTrainNumber(String tn, String d) {
  lcd.print(tn);
  lcd.setCursor(0, 1);
  lcd.print(d);
}


void loop() {
  int lastStationState = digitalRead(lastStationPin);
  int nextStationState = digitalRead(nextStationPin);
  int callStationState = digitalRead(callStationPin);
  
  if(lastStationState != prevLastStation && lastStationState == HIGH && stationNumber > 0) {
    stationNumber -= 1;
    prevLastStation = lastStationState;
  }
  if(nextStationState != prevNextStation && nextStationState == HIGH) {
    stationNumber++;
    prevNextStation = nextStationState;
  }
  if(callStationState != prevCallStation && callStationState == HIGH) {
    callingStation = !callingStation;
    prevCallStation = callStationState;
  }

  if(callingStation == 1) {
    lcd.print("RB 2");
    lcd.setCursor(0,1);
    lcd.print("Mannheim");
    Serial.println("calling");
    delay(3000);
  }
}
