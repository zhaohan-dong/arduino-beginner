#include <LiquidCrystal.h>
// Piezo to pin 6, select switch to pin 10, confirm switch to pin 9



LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int selectPin = 10;
const int confirmPin = 9;
const int piezoPin = 6;
int prevSelectState;
int prevConfirmState;

void setup() {
  lcd.begin(16,2);
}

void loop() {
  int note;
  int notedigit[] = {0, 0, 0, 0};
  int playing = 0;
  int order = 0;
  lcd.clear();
  lcd.print("Tone Generator");
  lcd.setCursor(0, 1);
  lcd.print("____Hz");
  lcd.setCursor(0, 1);
  lcd.cursor();
  while(playing == 0) {
    note = 0;
    int selectState = digitalRead(selectPin);
    int confirmState = digitalRead(confirmPin);
    lcd.setCursor(order, 1);
    lcd.print(notedigit[order]);
    if(prevSelectState != selectState) {
      if(selectState == LOW) {
        if(notedigit[order] < 9) {
          notedigit[order]++;
        } else {
          notedigit[order] = 0;
        }
      }
    }
    if(prevConfirmState != confirmState) {
      if(confirmState == LOW) {
        order++;
      }
    }
    prevSelectState = selectState;
    prevConfirmState = confirmState;
    if(order == 4) {
      note = 1000 * notedigit[0] + 100 * notedigit[1] + 10 * notedigit[2] + notedigit[3];
      playing = 1;
      }
  }
  if(playing == 1) {
    lcd.clear();
    lcd.noCursor();
    lcd.print("Playing");
    lcd.setCursor(0, 1);
    lcd.print(note);
    lcd.print("Hz");
    tone(piezoPin, note, 2000);
    delay(2000);
  }
}
