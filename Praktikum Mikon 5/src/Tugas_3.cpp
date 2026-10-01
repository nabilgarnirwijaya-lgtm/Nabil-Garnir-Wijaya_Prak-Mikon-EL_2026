#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "skibidi";
const char* password = "Skibiditoilet";
const String url = "https://data.bmkg.go.id/DataMKG/TEWS/autogempa.json";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());
    Serial.print("Fetching " + url + "... ");
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        Serial.print("HTTP ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println();
        Serial.println(payload);
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        Serial.println(":-(");
    }
}
void loop() {
    delay(100);
}
