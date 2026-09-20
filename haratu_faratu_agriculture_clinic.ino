#include <WiFi.h>
#include <WebServer.h>

// SMART AGRICULTURE CLINIC
// Agriculture Clinic - Smart Agriculture Project

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

#define SOIL_PIN 34
#define RELAY_PIN 27

WebServer server(80);

int getMoisture() {
  int raw = analogRead(SOIL_PIN);
  int moisture = map(raw, 2550, 881, 0, 90);
  return constrain(moisture, 0, 90);
}

void handleRoot() {
  int moisture = getMoisture();
  String irrigationStatus;
  String pumpStatus;

  if (moisture < 40) {
    digitalWrite(RELAY_PIN, LOW);
    pumpStatus = "ON";
    irrigationStatus = "સિંચાઈ જરૂરી";
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    pumpStatus = "OFF";
    irrigationStatus = "માટીની ભેજ યોગ્ય";
  }

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>Agriculture Clinic</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#f5f7fa;padding:20px}";
  html += ".card{max-width:500px;margin:auto;background:white;padding:25px;border-radius:18px;box-shadow:0 3px 12px #ccc}";
  html += "h1{font-size:26px}.value{font-size:48px;font-weight:bold}.status{font-size:22px;margin:15px 0}.info{font-size:18px}</style>";
  html += "</head><body><div class='card'>";
  html += "<h1>Agriculture Clinic</h1><p>Smart Agriculture Project</p><hr>";
  html += "<div class='info'>Soil Moisture</div>";
  html += "<div class='value'>" + String(moisture) + "%</div>";
  html += "<div class='status'>" + irrigationStatus + "</div>";
  html += "<div class='info'>Water Pump: <b>" + pumpStatus + "</b></div>";
  html += "<hr><p>Automatic irrigation system</p>";
  html += "</div></body></html>";

  server.send(200, "text/html; charset=utf-8", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Pump OFF at startup
  digitalWrite(RELAY_PIN, HIGH);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
