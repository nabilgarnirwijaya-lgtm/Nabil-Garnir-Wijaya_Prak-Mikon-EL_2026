#include <Arduino.h>

const int potensiometerPin = 34;      
const float V_REF = 3.3;      
const int ADC_RESOLUTION = 4095; 
const float rMax = 10000.0; 
const int ledPin_green = 25;
const int ledPin_yellow = 26;
const int ledPin_red = 27;
const int button_pin = 14;

void setup() {
        Serial.begin(115200);
        analogReadResolution(12); 
        pinMode(ledPin_green, OUTPUT);
        pinMode(ledPin_yellow, OUTPUT);
        pinMode(ledPin_red, OUTPUT);
        pinMode(button_pin, INPUT_PULLUP);
}
void loop() {
        int rawADC = analogRead(potensiometerPin);
        int buttonState = digitalRead(button_pin);
        float voltage = (rawADC / (float)ADC_RESOLUTION) * V_REF;
        float resistance = (rawADC / (float)ADC_RESOLUTION) * rMax;

        Serial.print("Raw ADC: ");
        Serial.print(rawADC);
        Serial.print("\tTegangan: ");
        Serial.print(voltage, 3);
        Serial.print(" V\tHambatan: ");
        Serial.print(resistance, 1);
        Serial.println(" Ohm");

        if (buttonState == LOW) {
                Serial.println("Led mati");
                digitalWrite(ledPin_red, LOW);
                digitalWrite(ledPin_green, LOW);
                digitalWrite(ledPin_yellow, LOW);
        }
        else {
                if (voltage >= 2.5) {
                Serial.println("Led hijau menyala");
                digitalWrite(ledPin_red, LOW);
                digitalWrite(ledPin_green, HIGH);
                digitalWrite(ledPin_yellow, LOW);
                } else if (voltage < 2.5 && voltage>= 1.3) {
                Serial.println("Led kuning menyala");
                digitalWrite(ledPin_red, LOW);
                digitalWrite(ledPin_green, LOW);
                digitalWrite(ledPin_yellow, HIGH);
                } else if (voltage < 1.3 && voltage >= 0) {
                Serial.println("Led merah menyala");
                digitalWrite(ledPin_red, HIGH);
                digitalWrite(ledPin_green, LOW);
                digitalWrite(ledPin_yellow, LOW);
        } }  
        delay(100);
}

