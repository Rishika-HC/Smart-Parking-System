#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "abc"
#define WIFI_PASSWORD "def"

#define TRIG_PIN_1 19
#define ECHO_PIN_1 18
#define TRIG_PIN_2 26
#define ECHO_PIN_2 27

const char* serverURL = "http://10.235.104.217:3000/api/parking"; // Change this

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.println(WiFi.localIP());
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration == 0 ? -1 : duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN_1, OUTPUT); pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT); pinMode(ECHO_PIN_2, INPUT);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🔄 Reconnecting to WiFi...");
    connectToWiFi();
  }

  long d1 = getDistance(TRIG_PIN_1, ECHO_PIN_1);
  long d2 = getDistance(TRIG_PIN_2, ECHO_PIN_2);

  String s1 = (d1 > 0 && d1 <= 30) ? "Occupied" : "Available";
  String s2 = (d2 > 0 && d2 <= 30) ? "Occupied" : "Available";

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"slot1\":\"" + s1 + "\", \"slot2\":\"" + s2 + "\"}";
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.println("✅ Data sent: " + payload);
    Serial.println("🔁 Server response: " + http.getString());
  } else {
    Serial.println("❌ HTTP POST failed. Error: " + http.errorToString(httpResponseCode));
  }

  http.end();
  delay(2000);
}
