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
                      "Graben-Neudorf", "Wiesental", "Wagh""\xe0""usel", "Neulußheim", "Hockenheim",
                      "Oftersheim", "Schwetzingen", "Mannheim-Rheinau", "Mannheim-Neckarau", "Mannheim Hbf"};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(lastStationPin, INPUT);
  pinMode(nextStationPin, INPUT);
  pinMode(callStationPin, INPUT);
  lcd.clear();
}

int printTrainNumber(String trainNumber, String dir) {
  lcd.clear();
  lcd.print(trainNumber);
  lcd.setCursor(0, 1);
  lcd.print("nach ");
  lcd.print(dir);
  if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
    else {
      delay(3000);
  }
  if(dir.length() > 11) {
    for (int i = 0; i < dir.length() - 11; i++) {
      lcd.scrollDisplayLeft();
      lcd.home();
      lcd.print(trainNumber);
      if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
        break;
      }
      else {
        delay(3000);
      }
    }
  }
}

int callStation(String trainNumber, String dir, String stationName) {
  lcd.clear();
  lcd.home();
  lcd.print(stationName);
  if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
   else {
      delay(3000);
   }
  if(stationName.length() > 16) {
    for (int i = 0; i < stationName.length() - 16; i++) {
      lcd.scrollDisplayLeft();
      if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
        break;
      }
      else {
        delay(500);
      }
    }
    if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
    else {
      delay(1000);
     }
  }
  printTrainNumber(trainNumber, dir);
  if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
  } else {
    delay(3000);
  }
}

int printNextStation(String stationName) {
  lcd.clear();
  lcd.home();
  lcd.print("N""\xe1""chste Station:");
  lcd.setCursor(0, 1);
  lcd.print(stationName);
  if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
    else {
      delay(3000);
  }
  if(stationName.length() > 16) {
    for (int i = 0; i < stationName.length() - 16; i++) {
      lcd.scrollDisplayLeft();
      if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
        break;
      } else {
      delay(500);
  }
    }
    if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
    else {
      delay(1000);
  }
  }
}

void loop() {
  int lastStationState = digitalRead(lastStationPin);
  int nextStationState = digitalRead(nextStationPin);
  int callStationState = digitalRead(callStationPin);
  
  if(lastStationState != prevLastStation) {
    if(lastStationState == HIGH) {
          stationNumber -= 1;
    }
    prevLastStation = lastStationState;
  }
  if(nextStationState != prevNextStation) {
    if(nextStationState == HIGH) {
      stationNumber++;
    }
    prevNextStation = nextStationState;
  }
  if(callStationState != prevCallStation) {
    if(callStationState == HIGH && callingStation == true) {
      callingStation = false;
      stationNumber++;
    } else if(callStationState == HIGH && callingStation == false) {
      callingStation = true;
    }
    prevCallStation = callStationState;
  }

  if(callingStation == true) {
    callStation(trainNumber, dir, station[stationNumber]);
    Serial.print(callingStation);
    Serial.print(stationNumber);
    Serial.println("calling");
  } else {
    printNextStation(station[stationNumber]);
    printTrainNumber(trainNumber, dir);
    Serial.print(callingStation);
    Serial.print(stationNumber);
    Serial.println("next Sta");
    if(digitalRead(lastStationPin) || digitalRead(nextStationPin) || digitalRead(callStationPin)) {
    }
    else {
      delay(3000);
      }
    }
  prevLastStation = lastStationState;
  prevNextStation = nextStationState;
  prevCallStation = callStationState;
}
