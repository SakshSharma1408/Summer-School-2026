const int ENA = 9;
const int IN1 = 7;
const int IN2 = 8;
const int potPin = A0;
const int dirButton = 2;
const int startButton = 3;

bool direction = true;
bool motorRunning = true;

bool lastDir = HIGH;
bool lastStart = HIGH;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(dirButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  Serial.begin(9600);}

void loop() {
  int pot = analogRead(potPin);
  int pwm = map(pot,0,1023,0,255);
  int speedPercent = map(pwm,0,255,0,100);
  bool dirNow = digitalRead(dirButton);
  if(lastDir==HIGH && dirNow==LOW){
    direction=!direction;
    delay(200);}

  lastDir=dirNow;
  bool startNow = digitalRead(startButton);

  if(lastStart==HIGH && startNow==LOW){
    motorRunning=!motorRunning;
    delay(200);}

  lastStart=startNow;

  if(motorRunning){
    analogWrite(ENA,pwm);
    if(direction){
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);}

    else{
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);}}

  else{
    analogWrite(ENA,0);}

  Serial.print("Direction: ");

  if(direction)
    Serial.print("Forward");
  else
    Serial.print("Reverse");

  Serial.print(" | Speed: ");
  Serial.print(speedPercent);
  Serial.print("%");
  Serial.print(" | State: ");

  if(motorRunning)
    Serial.println("Running");
  else
    Serial.println("Stopped");

  delay(300);}