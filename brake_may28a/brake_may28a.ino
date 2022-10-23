#include<LiquidCrystal.h>
const int emergencyPin = 2;
const int awsPin = 10;
const int awsLed = 9;
const int piezoPin = 11;
const int brakePin = A0;
const int awsSigPin = 12;
int prevAwsReset;
volatile int awsReset;
volatile int prevAwsStatus;
String disp;
String prevDisp;
int awsStatus;
unsigned long timer;
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);


void setup() {
  lcd.begin(16, 2);
  pinMode(emergencyPin, INPUT);
  pinMode(awsPin, INPUT);
  pinMode(awsLed, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(brakePin, INPUT);
  pinMode(awsSigPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(emergencyPin), emergencyBrake, RISING); // trigger emergency brake whenever the pin is pushed
}

int brakeStatus() {
  int potVal = analogRead(brakePin);
  return map(potVal, 0, 1023, 0, 130);
}

void displayStatus(int brakePerc) {
  if(brakePerc < 10) {
    disp = "Release";
  } else if(brakePerc >= 10 && brakePerc < 110) {
    disp = "Apply " + String(brakePerc-10) + "%";
  } else if(brakePerc >= 110 && brakePerc < 120) {
    disp = "Full Service";
  } else if(brakePerc >= 120) {
    disp = "Emergency Brake";
  }
  if(prevDisp != disp) {
    lcd.clear();
    lcd.print(disp);
  }
  prevDisp = disp;
}

void emergencyBrake() {
  prevAwsStatus = 1;
  while(1) {
    awsStatus = aws(prevAwsStatus);
    prevAwsStatus = awsStatus;
    displayStatus(130);
    if(brakeStatus() < 10 && awsStatus == 0) {
      break;
    }
  }
}

int aws(int prevAwsStatus) {
  int sig = digitalRead(awsSigPin);
  awsReset = digitalRead(awsPin);
  if (prevAwsReset != awsReset && awsReset == LOW) { // AWS reset on button release
    sig = LOW;
    noTone(piezoPin);
  } else if(prevAwsStatus == 1) { // if AWS was on and not reset, signal is high
    sig = HIGH;
  }
  
  if(sig == LOW) {
    digitalWrite(awsLed, LOW);
    if(prevAwsStatus == 1) { // sound clear tone when previously aws was activated
      tone(piezoPin, 587.32, 500);
    }
    prevAwsReset = awsReset;
    return 0;
  }
  if(sig == HIGH) {
    digitalWrite(awsLed, HIGH);
    tone(piezoPin, 440);
    prevAwsReset = awsReset;
    return 1;
  }
}

void loop() {
  awsStatus = aws(prevAwsStatus);
  if(awsStatus != prevAwsStatus) { // start timer when aws first triggered
    if(awsStatus == 1){
      timer = millis();
    }
  }
  prevAwsStatus = awsStatus;  // set previous AWS status to feed back to function
  if(millis() - timer > 2000 && awsStatus == 1) { // apply emergency brake when no response
    emergencyBrake();
  }
  displayStatus(brakeStatus());
}
