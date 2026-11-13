#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <CTBot.h>

Adafruit_BME280 bme;
CTBot myBot;

String ssid   = "skibidi";
String pass   = "Skibiditoilet";
String token  = "9068837372:AAHO92l4li96sSR_t5wZ4NIIa8V_gJvwB44";
String apiKey = "GZQ02W15OTRI82BG";

const int pinLED = 18;
const unsigned long intervalThingspeak = 20000;
unsigned long lastSend = 0;

void kirimThingspeak(float suhu, float kelembapan) {
  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
               "&field1=" + String(suhu) +
               "&field2=" + String(kelembapan);

  http.begin(url);
  int httpCode = http.GET();
  String respon = http.getString();
  http.end();

  if (httpCode == 200 && respon != "0") {
    Serial.println(">> ThingSpeak terkirim, entry ke-" + respon);
  } else {
    Serial.println(">> ThingSpeak GAGAL (kode " + String(httpCode) + ")");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);

  Wire.begin();
  bme.begin(0x76);

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  Serial.println(myBot.testConnection() ? "Telegram OK" : "Telegram GAGAL");
}

void loop() {
  if (millis() - lastSend >= intervalThingspeak) {
    lastSend = millis();
    float suhu = bme.readTemperature();
    float kelembapan = bme.readHumidity();
    kirimThingspeak(suhu, kelembapan);
  }

  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    Serial.print("Pesan masuk: ");
    Serial.println(msg.text);

    if (msg.text.equalsIgnoreCase("LED ON")) {
      digitalWrite(pinLED, HIGH);
      myBot.sendMessage(msg.sender.id, "LED menyala");
    } else if (msg.text.equalsIgnoreCase("LED OFF")) {
      digitalWrite(pinLED, LOW);
      myBot.sendMessage(msg.sender.id, "LED mati");
    } else if (msg.text.equalsIgnoreCase("BACA SUHU")) {
      float suhu = bme.readTemperature();
      String reply = "Suhu: " + String(suhu, 1) + " C";
      myBot.sendMessage(msg.sender.id, reply);
    } else if (msg.text.equalsIgnoreCase("BACA KELEMBAPAN")) {
      float kelembapan = bme.readHumidity();
      String reply = "Kelembapan: " + String(kelembapan, 0) + " %";
      myBot.sendMessage(msg.sender.id, reply);
    } else {
      String reply = "Perintah: LED ON / LED OFF\nBACA SUHU / BACA KELEMBAPAN";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }
  delay(50);
}
