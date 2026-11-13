#include <Arduino.h>
#include <WiFi.h>
#include <CTBot.h>

CTBot myBot;

String ssid   = "skibidi";
String pass   = "Skibiditoilet";
String token  = "9068837372:AAHO92l4li96sSR_t5wZ4NIIa8V_gJvwB44";

const int pinLED = 18;

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("Telegram OK");
  else
    Serial.println("Telegram GAGAL");
}

void loop() {
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
    } else {
      myBot.sendMessage(msg.sender.id, "Kirim LED ON atau LED OFF");
    }
  }
  delay(500);
}
