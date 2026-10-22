#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "skibidi";
const char* password = "Skibiditoilet";

WebServer server(80);
Servo myServo;
const int servoPin = 26;

const char halaman[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Kontrol Servo ESP32</title>
</head>
<body style="font-family:Arial;text-align:center;margin-top:50px">
<h2>Kendali Servo ESP32</h2>
<p>Sudut: <span id="pos">90</span>&deg;</p>
<input type="range" min="0" max="180" value="90"
 oninput="gerak(this.value)">
<p id="status">Siap</p>
<script>
function gerak(val) {
  document.getElementById("pos").innerText = val;
  fetch("/servo?pos=" + val)
    .then(r => r.text())
    .then(t => document.getElementById("status").innerText = t)
    .catch(e => document.getElementById("status").innerText = "Gagal");
}
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
  if (!server.hasArg("pos")) {
    server.send(400, "text/plain", "Parameter pos tidak ditemukan");
    return;
  }

  String nilai = server.arg("pos");

  if (!isNumeric(nilai)) {
    server.send(400, "text/plain", "Input harus angka");
    return;
  }

  int pos = nilai.toInt();

  if (pos < 0 || pos > 180) {
    server.send(400, "text/plain", "Sudut harus 0-180");
    return;
  }

  myServo.write(pos);
  server.send(200, "text/plain",
              "Servo bergerak ke " + String(pos) + " derajat");
}

void setup() {
  Serial.begin(115200);

  myServo.attach(servoPin);
  myServo.write(90);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/servo", handleServo);
  server.begin();
}

void loop() {
  server.handleClient();
}