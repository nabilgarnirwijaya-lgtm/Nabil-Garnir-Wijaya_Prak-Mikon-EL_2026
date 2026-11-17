#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
const int in1 = 17;
const int in2 = 18;
const int pwmPin = 23;
const int stby = 19;
const int pot = 16;

void setup() {
 Serial.begin(115200);
 ina219.begin();
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 pinMode(stby, OUTPUT);
 digitalWrite(stby, HIGH); 
 digitalWrite(in1, HIGH); 
 digitalWrite(in2, LOW);
 ledcSetup(0, 5000, 8);
 ledcAttachPin(pwmPin, 0);
 delay(1000);
}
void loop() {
 float arus = ina219.getCurrent_mA();
 float tegangan_V = ina219.getBusVoltage_V();
 float daya_mW = ina219.getPower_mW();
 int potValue = analogRead(pot);
 int pwmValue = map(potValue, 0, 4095, 0, 255);
 int duty = pwmValue;
 ledcWrite(0, duty);
 Serial.print("Arus: ");
 Serial.print(arus);
 Serial.println(" mA");
 Serial.print("Tegangan: ");
 Serial.print(tegangan_V);
 Serial.println(" V");
 Serial.print("Daya: ");
 Serial.print(daya_mW);
 Serial.println(" mW");
 delay(1000);
}