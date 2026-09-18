#include <Arduino.h>
const int in1 = 25;
const int in2 = 27;
const int pwmPin = 26;
const int stby = 12;
const int pot = 34;


void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(stby, OUTPUT);
  analogReadResolution(12);
  digitalWrite(stby, HIGH); 
  digitalWrite(in1, HIGH);  
  digitalWrite(in2, LOW);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(pwmPin, 0);
}

void loop() {
  int RawADC = analogRead(pot);
  int dutyCycle = map(RawADC, 0, 4095, 0, 255);
  ledcWrite(0, dutyCycle);
}