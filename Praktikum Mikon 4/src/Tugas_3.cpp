#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_INA219 ina219;
#define BUZZER_PIN 26
void setup() {
Serial.begin(115200);
ina219.begin();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
pinMode(BUZZER_PIN, OUTPUT);
}
void loop() {
float arus = ina219.getCurrent_mA();
float tegangan_V = ina219.getBusVoltage_V();
float daya_mW = ina219.getPower_mW();

Serial.print("Arus (mA): ");
Serial.println(arus);
Serial.print("Tegangan (V): ");
Serial.println(tegangan_V);
Serial.print("daya (mW): ");
Serial.println(daya_mW);

if (arus > 500.0) {
digitalWrite(BUZZER_PIN, HIGH);
display.clearDisplay();
display.setTextSize(1);        
display.setTextColor(WHITE);
display.setCursor(0, 0); 
display.print("Arus (mA): ");
display.println(arus);
display.print("Tegangan (V): ");
display.println(tegangan_V);
display.print("daya (mW): ");
display.println(daya_mW);
display.print("Status: OVERLOAD");
display.display();
}
else {
digitalWrite(BUZZER_PIN, LOW);
display.clearDisplay();
display.setTextSize(1);        
display.setTextColor(WHITE);
display.setCursor(0, 0); 
display.print("Arus (mA): ");
display.println(arus);
display.print("Tegangan (V): ");
display.println(tegangan_V);
display.print("daya (mW): ");
display.println(daya_mW);
display.print("Status: NORMAL");
display.display();
}
delay(500);
}