#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ2_PIN 34

#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27
#define BUZZER 14

int baseline = 0;

void setColor(bool r, bool g, bool b) {

  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("Stabilizing MQ-2... (10 sec)");
  delay(10000);

  int sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += analogRead(MQ2_PIN);
    delay(200);
  }
  baseline = sum / 30;

  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.println("Timestamp, MQ-2 Value, DHT11 Temp, DHT11 Humidity, Severity Level");
}

void loop() {

  int gasRaw = analogRead(MQ2_PIN);
  int diff = gasRaw - baseline;

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT ERROR");
    delay(2000);
    return;
  }

  String status;

  // THRESHOLDS
  if (diff < 80) {
    status = "SAFE";
    setColor(LOW, HIGH, LOW);   // GREEN
    noTone(BUZZER);
  }

  else if (diff < 250) {
    status = "MODERATE";
    setColor(HIGH, HIGH, LOW);  // YELLOW

    tone(BUZZER, 1000);
    delay(200);
    noTone(BUZZER);
  }

  else {
    status = "DANGER";
    setColor(HIGH, LOW, LOW);   // RED

    tone(BUZZER, 2000);
  }

  // TIME
  int seconds = millis() / 1000;
  int mins = seconds / 60;
  int secs = seconds % 60;

  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", mins, secs);

  // CSV
  Serial.print(timeStr);
  Serial.print(", ");
  Serial.print(gasRaw);
  Serial.print(" ppm, ");
  Serial.print(temp);
  Serial.print("°C, ");
  Serial.print(hum);
  Serial.print("%, ");
  Serial.println(status);

  delay(2000);
}