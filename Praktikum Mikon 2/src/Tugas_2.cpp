#include <Arduino.h>

const int LED_PIN1 = 27;
const int LED_PIN2 = 26;
const int LED_PIN3 = 25;
const int BUTTON = 32;

void setup() {
        pinMode(LED_PIN1, OUTPUT);
        pinMode(LED_PIN2, OUTPUT);
        pinMode(LED_PIN3, OUTPUT);
        pinMode(BUTTON, INPUT_PULLUP);
        Serial.begin(9600);
}
void loop() {
        int button = digitalRead(BUTTON);
        if (button == HIGH) {
                digitalWrite(LED_PIN1, LOW);
                digitalWrite(LED_PIN2, LOW);
                digitalWrite(LED_PIN3, LOW);
                Serial.println("LED OFF");
        } else{
                digitalWrite(LED_PIN1, HIGH);
                digitalWrite(LED_PIN2, HIGH);
                digitalWrite(LED_PIN3, HIGH);
                Serial.println("LED ON");
        }
delay(500);
}
