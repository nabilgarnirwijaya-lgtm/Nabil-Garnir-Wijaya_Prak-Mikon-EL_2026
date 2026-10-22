#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>
const char* ssid = "skibidi";
const char* password = "Skibiditoilet";
WebServer server(80);
Servo myServo;
Adafruit_MPU6050 mpu;
const int servoPin = 26;
const int buzzerPin = 27;
int currentServoPos = 90;
float accYTerakhir = 0.0;
float sudutTerakhir = 0.0;
String statusTerakhir = "";
String buzzerTerakhir = "OFF";
unsigned long waktuSebelumnya = 0;
const long interval = 2000;
float bacaKemiringan(float &accY) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accY = a.acceleration.y;
  float roll = atan2(a.acceleration.y,
                     a.acceleration.z) * 180.0 / PI;
  return fabs(roll);
}
void cekKondisiMPU() {
  sudutTerakhir = bacaKemiringan(accYTerakhir);
  String statusBaru;
  if (sudutTerakhir > 35.0) {
    statusBaru = "BAHAYA";
    buzzerTerakhir = "ON (MENYALA)";
    digitalWrite(buzzerPin, HIGH);
    currentServoPos = 180;
    myServo.write(currentServoPos);
  }
  else if (sudutTerakhir >= 15.0) {
    statusBaru = "SEDANG";
    buzzerTerakhir = "OFF";
    digitalWrite(buzzerPin, LOW);
    if (statusTerakhir != "SEDANG") {
      currentServoPos = 90;
      myServo.write(currentServoPos);
    }
  }
  else {
    statusBaru = "DATAR";
    buzzerTerakhir = "OFF";
    digitalWrite(buzzerPin, LOW);
    if (statusTerakhir != "DATAR") {
      currentServoPos = 0;
      myServo.write(currentServoPos);
    }
  }
  statusTerakhir = statusBaru;
  Serial.printf("Accel Y: %.2f | Sudut: %.1f | Status: %s | ",
                accYTerakhir, sudutTerakhir,
                statusTerakhir.c_str());
  Serial.printf("Buzzer: %s | Servo: %d\n",
                buzzerTerakhir.c_str(), currentServoPos);
}
const char halaman[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MPU6050 Servo Buzzer</title>
</head>
<body style="font-family:Arial;text-align:center;margin-top:30px">
<h2>Sistem Otomatis MPU6050</h2>
<p>Sudut: <b id="sudut">--</b>&deg;</p>
<p>Accel Y: <b id="accY">--</b> m/s2</p>
<p>Status: <b id="status">MEMUAT</b></p>
<p>Buzzer: <b id="buzzer">--</b></p>
<hr>
<h3>Kendali Servo Manual</h3>
<p>Sudut servo: <b id="pos">90</b>&deg;</p>
<input type="range" min="0" max="180" value="90"
 oninput="gerak(this.value)">
<p id="pesan"></p>
<script>
function gerak(v) {
  document.getElementById("pos").innerText = v;
  fetch("/servo?pos=" + v)
    .then(r => r.text())
    .then(t => document.getElementById("pesan").innerText = t);
}
function ambilData() {
  fetch("/data")
    .then(r => r.json())
    .then(d => {
      document.getElementById("sudut").innerText =
        Number(d.sudut).toFixed(1);
      document.getElementById("accY").innerText =
        Number(d.accY).toFixed(2);
      document.getElementById("status").innerText = d.status;
      document.getElementById("buzzer").innerText = d.buzzer;
      document.getElementById("pos").innerText = d.servo;
    });
}
setInterval(ambilData, 2000);
ambilData();
</script>
</body>
</html>
)rawliteral";
bool isNumeric(String s) {
  if (s.length() == 0) return false;
  for (unsigned int i = 0; i < s.length(); i++) {
    if (!isDigit(s.charAt(i))) return false;
  }
  return true;
}
void handleRoot() {
server.send(200, "text/html", halaman);
}
void handleServo() {
  if (!server.hasArg("pos") ||
      !isNumeric(server.arg("pos"))) {
    server.send(400, "text/plain", "Input harus angka");
    return;
  }
  int pos = server.arg("pos").toInt();
  if (pos < 0 || pos > 180) {
    server.send(400, "text/plain", "Sudut harus 0-180");
    return;
  }
  currentServoPos = pos;
  myServo.write(currentServoPos);
  server.send(200, "text/plain",
              "Servo diatur ke " + String(pos) + " derajat");
}
void handleData() {
  String json = "{\"sudut\":" + String(sudutTerakhir, 2) +
                ",\"accY\":" + String(accYTerakhir, 2) +
                ",\"status\":\"" + statusTerakhir +
                "\",\"buzzer\":\"" + buzzerTerakhir +
                "\",\"servo\":" + String(currentServoPos) + "}";
  server.send(200, "application/json", json);
}
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  myServo.attach(servoPin);
  myServo.write(currentServoPos);
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
  cekKondisiMPU();
  server.on("/", handleRoot);
  server.on("/servo", handleServo);
 server.on("/data", handleData);
  server.begin();
}
void loop() {
  server.handleClient();
  unsigned long sekarang = millis();
  if (sekarang - waktuSebelumnya >= interval) {
    waktuSebelumnya = sekarang;
    cekKondisiMPU();
  }
}
