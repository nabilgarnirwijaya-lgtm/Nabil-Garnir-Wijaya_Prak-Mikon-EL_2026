#include <Arduino.h>

const int potensiometerPin = 34;      
const float V_REF = 3.3;      
const int ADC_RESOLUTION = 4095; 
const float rMax = 10000.0; 

void setup() {
        Serial.begin(115200);
        analogReadResolution(12); 
}

void loop() {
        int rawADC = analogRead(potensiometerPin);
        float voltage = (rawADC / (float)ADC_RESOLUTION) * V_REF;
        float resistance = (rawADC / (float)ADC_RESOLUTION) * rMax;
        Serial.print("Raw ADC: ");
        Serial.print(rawADC);
        Serial.print("\tTegangan: ");
        Serial.print(voltage, 3);
        Serial.print(" V\tHambatan: ");
        Serial.print(resistance, 1);
        Serial.println(" Ohm");
        delay(500);
}
