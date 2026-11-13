#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

const char* ssid = "skibidi";
const char* password = "Skibiditoilet";

String apiKey = "GZQ02W15OTRI82BG";  // GANTI dengan API Key kalian

const unsigned long intervalThingspeak = 20000;
unsigned long lastSend = 0;


void setup() {
  Serial.begin(115200);
  Serial.print("Koneksi WiFi");
  Wire.begin();
  bme.begin(0x76);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");
}

void loop() {
  if (millis() - lastSend >= intervalThingspeak) {
    lastSend = millis();

    float suhu = bme.readTemperature();
    float kelembapan = bme.readHumidity();

    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" C | Kelembapan: ");
    Serial.println(kelembapan);

    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(suhu) +
                 "&field2=" + String(kelembapan);

    http.begin(url);
    int httpCode = http.GET();
    String respon = http.getString();
    http.end();

    if (httpCode == 200 && respon != "0") {
      Serial.println("Data terkirim ke ThingSpeak, entry ke-" + respon);
    } else {
      Serial.println("Gagal kirim ke ThingSpeak (kode " + String(httpCode) + ")");
    }
  }
}
