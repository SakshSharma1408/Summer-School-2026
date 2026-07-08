#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------------- THINGSPEAK ----------------
String apiKey = "APIKEY";
const char* server = "http://api.thingspeak.com/update";

#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER 4

#define LED_GREEN 12
#define LED_YELLOW 13
#define LED_RED 14

float readings[5];
int indexNum = 0;

void setup_wifi() {
  Serial.print("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  return distance;
}

// ---------------- AVERAGE ----------------
float getAverage(float val) {
  readings[indexNum] = val;
  indexNum = (indexNum + 1) % 5;

  float sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += readings[i];
  }
  return sum / 5;
}

void sendToThingSpeak(float distance, int levelValue) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(distance) +
                 "&field2=" + String(levelValue);

    Serial.println("Sending to TS:");
    Serial.println(url);   // DEBUG

    http.begin(url);
    int httpCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpCode);

    http.end();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  for (int i = 0; i < 5; i++) readings[i] = 30;

  setup_wifi();

  // Buzzer test
  tone(BUZZER, 1000);
  delay(300);
  noTone(BUZZER);
}

void loop() {

  float rawDistance = getDistance();
  float avgDistance = getAverage(rawDistance);

  Serial.print("Raw: ");
  Serial.print(rawDistance);
  Serial.print(" cm | Avg: ");
  Serial.println(avgDistance);

  String level;
  int levelValue;

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);

  // 🔴 CRITICAL (<15 cm) → instant

if (rawDistance < 15) {
  level = "CRITICAL";
  levelValue = 2;

  digitalWrite(LED_RED, HIGH);
  tone(BUZZER, 3000);
}

else if (rawDistance >= 15 && rawDistance <= 30) {
  level = "WARNING";
  levelValue = 1;

  digitalWrite(LED_YELLOW, HIGH);

  tone(BUZZER, 2000);
  delay(200);
  noTone(BUZZER);
}

else {
  level = "NORMAL";
  levelValue = 0;

  digitalWrite(LED_GREEN, HIGH);
}

  Serial.println("Level: " + level);

  // 📡 Send to ThingSpeak
  sendToThingSpeak(avgDistance, levelValue);

  // ⚠️ IMPORTANT: ThingSpeak limit
  delay(15000);
}