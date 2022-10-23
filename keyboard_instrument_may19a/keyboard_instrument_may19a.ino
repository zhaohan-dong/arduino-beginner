const float notes[] = {440, 466.16, 493.88, 523.25, 554.36, 587.32, 622.26, 659.26, 698.46};
void setup() {
  Serial.begin(9600);
}

void loop() {
  int keyVal = analogRead(A0);
  Serial.println(keyVal);
  if(keyVal < 50) {
    // Af
    tone(8, notes[0]);
  } else if(keyVal >= 50 && keyVal < 92) {
    // B
    tone(8, notes[2]);
  } else if(keyVal >= 92 && keyVal < 134) {
    // A#
    tone(8, notes[1]);
  } else if (keyVal >= 134 && keyVal < 339) {
    // C
    tone(8, notes[3]);
  } else if (keyVal >= 339 && keyVal < 526) {
    // D
    tone(8, notes[5]);
  } else if (keyVal >= 526 && keyVal < 597) {
    // C#
    tone(8, notes[4]);
  } else if (keyVal >= 597 && keyVal < 690) {
    // E
    tone(8, notes[7]);
  } else if (keyVal >= 690 && keyVal < 773) {
    // D#
    tone(8, notes[6]);
  } else if (keyVal >= 773) {
    // F
    tone(8, notes[8]);
  } else {
    noTone(8);
  }

}
