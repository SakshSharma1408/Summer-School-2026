const int red = 13;
const int yellow = 12;
const int green = 11;
const int button = 7;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(button, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  if (digitalRead(button) == LOW) {
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);

    Serial.print(millis());
    Serial.println(" ms : Pedestrian Crossing");

    delay(8000);
    return;
  }

  digitalWrite(red, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);

  Serial.print(millis());
  Serial.println(" ms : RED");

  delay(5000);

  if (digitalRead(button) == LOW) return;

  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, LOW);

  Serial.print(millis());
  Serial.println(" ms : YELLOW");

  delay(2000);

  if (digitalRead(button) == LOW) return;

  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, HIGH);

  Serial.print(millis());
  Serial.println(" ms : GREEN");

  delay(4000);
}