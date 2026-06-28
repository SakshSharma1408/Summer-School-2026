const int trigPin = 9;
const int echoPin = 10;
const int yellowLED = 3;
const int redLED = 4;
const int buzzer = 8;

long duration;
float distance;

unsigned long previousMillis = 0;
bool state = LOW;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  ");

  unsigned long currentMillis = millis();

  if (distance > 50) {
    Serial.println("SAFE");
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    noTone(buzzer);}

  else if (distance > 20) {
    Serial.println("CAUTION");
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      state = !state;
      if (state)
        tone(buzzer,1000);
      else
        noTone(buzzer);}}

  else if (distance > 10) {
    Serial.println("WARNING");
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    if (currentMillis - previousMillis >= 200) {
      previousMillis = currentMillis;
      state = !state;
      if (state)
        tone(buzzer,1000);
      else
        noTone(buzzer);}}

  else {
    Serial.println("STOP!");
    if (currentMillis - previousMillis >= 100) {
      previousMillis = currentMillis;
      state = !state;

      digitalWrite(redLED,state);
      digitalWrite(yellowLED,state);

      if (state)
        tone(buzzer,1000);
      else
        noTone(buzzer);}}}