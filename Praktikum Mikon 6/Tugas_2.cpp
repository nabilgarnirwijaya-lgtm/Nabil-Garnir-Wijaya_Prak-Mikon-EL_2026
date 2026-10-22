#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

const char* ssid = "skibidi";
const char* password = "Skibiditoilet";
WebServer server(80);
Adafruit_MPU6050 mpu;
unsigned long waktuSebelumnya = 0;
const long interval = 2000;
float bacaKemiringan(float &nilaiAccelY) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  nilaiAccelY = a.acceleration.y;
  float roll = atan2(a.acceleration.y
                     a.acceleration.z) * 180.0 / PI;
  return fabs(roll);
}

String tentukanStatus(float sudut) {
  if (sudut > 35) return "BAHAYA";
  else if (sudut >= 15) return "SEDANG";
  else return "DATAR";
}

const char halaman[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Pemantauan MPU6050</title>
</head>
<body style="font-family:Arial;text-align:center;margin-top:50px">
<h2>Pemantauan Kemiringan MPU6050</h2>
<p>Sudut: <b id="sudut">--</b>&deg;</p>
<p>Accel Y: <b id="accY">--</b> m/s2</p>
<h3>Status: <span id="status">MEMUAT</span></h3>
<script>
function ambilData() {
  fetch("/data")
    .then(r => r.json())
    .then(d => {
      document.getElementById("sudut").innerText =
        Number(d.sudut).toFixed(1);
      document.getElementById("accY").innerText =
        Number(d.accY).toFixed(2);
      document.getElementById("status").innerText = d.status;
    });
}
setInterval(ambilData, 2000);
ambilData();
</script>
</body>
</html>
)rawliteral";

void handleData() {
  float accY = 0;
  float sudut = bacaKemiringan(accY);
  String status = tentukanStatus(sudut);
  String json = "{\"sudut\":" + String(sudut, 2) +
                ",\"accY\":" + String(accY, 2) +
                ",\"status\":\"" + status + "\"}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("MPU6050 tidak ditemukan!");
    while (true) delay(10);
  }
  Serial.println("MPU6050 siap");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());
  server.on("/", []() {
    server.send(200, "text/html", halaman);
  });
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long sekarang = millis();
  if (sekarang - waktuSebelumnya >= interval) {
    waktuSebelumnya = sekarang;
    float accY = 0;
    float sudut = bacaKemiringan(accY);
    Serial.print("Accel Y: ");
    Serial.print(accY, 2);
    Serial.print(" | Sudut: ");
    Serial.print(sudut, 1);
    Serial.print(" | Status: ");
    Serial.println(tentukanStatus(sudut));
  }
}
