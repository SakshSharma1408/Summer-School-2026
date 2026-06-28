#include <Servo.h>

Servo myServo;

const int potPin = A0;
const int buttonPin = 2;
bool lastButtonState = HIGH;

void setup() {
  myServo.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);}

void loop() {
  int potValue = analogRead(potPin);
  int angle = map(potValue, 0, 1023, 0, 180);
  myServo.write(angle);
  Serial.print("Servo Angle: ");
  Serial.println(angle);
  bool currentButton = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButton == LOW) {

    // Sweep 0° → 180°
    for (int i = 0; i <= 180; i++) {
      myServo.write(i);
      delay(10);}

    // Sweep 180° → 0°
    for (int i = 180; i >= 0; i--) {
      myServo.write(i);
      delay(10);}}

  lastButtonState = currentButton;
  delay(100);}