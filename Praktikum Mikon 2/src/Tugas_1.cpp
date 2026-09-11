#include <Arduino.h>

const int LED_PIN1= 32;
void setup() {
        pinMode(LED_PIN1, OUTPUT);
}

void loop() {
        digitalWrite(LED_PIN1, HIGH);
        delay(3000);
        digitalWrite(LED_PIN1, LOW);
        delay(3000);
}
