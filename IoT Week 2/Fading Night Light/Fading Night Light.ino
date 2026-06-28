const int led = 9;
const int button = 2;

int mode = 0;
bool lastButton = HIGH;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {

  bool current = digitalRead(button);

  if (lastButton == HIGH && current == LOW) {
    mode++;
    if (mode > 2) mode = 0;

    if (mode == 0) Serial.println("Mode 1 : Slow Breathing");
    if (mode == 1) Serial.println("Mode 2 : Fast Pulse");
    if (mode == 2) Serial.println("Mode 3 : SOS");

    delay(200);
  }

  lastButton = current;

  if (mode == 0) {

    for (int i = 0; i <= 255; i++) {
      analogWrite(led, i);
      delay(6);
    }

    for (int i = 255; i >= 0; i--) {
      analogWrite(led, i);
      delay(6);
    }

  }

  else if (mode == 1) {

    analogWrite(led,255);
    delay(250);
    analogWrite(led,0);
    delay(250);

  }

  else {

    for(int i=0;i<3;i++){
      analogWrite(led,255);
      delay(200);
      analogWrite(led,0);
      delay(200);
    }

    for(int i=0;i<3;i++){
      analogWrite(led,255);
      delay(600);
      analogWrite(led,0);
      delay(600);
    }

    for(int i=0;i<3;i++){
      analogWrite(led,255);
      delay(200);
      analogWrite(led,0);
      delay(200);
    }

    delay(1000);
  }

}