#include<Servo.h>
Servo door;
int tempsensor = A0;
int fuelsensor = A1;
int ldr = A2;

int irsensor = 2;
int lock = 3;
int unlock = 4;

int bled = 5; // cold
int gled = 6; // warm
int yled = 7; // hot    

int oled = 8; // headlight
int rled = 10; // fuel
int buzzer = 11;

void setup(){
  pinMode(bled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(yled, OUTPUT);
  pinMode(oled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(irsensor, INPUT);
  pinMode(lock, INPUT_PULLUP);
  pinMode(unlock, INPUT_PULLUP);

  digitalWrite(lock, HIGH);
  digitalWrite(unlock, HIGH);

  door.attach(9);
  door.write(0); 
  Serial.begin(9600);}

void loop(){
  int reading = analogRead(tempsensor);
  float voltage = reading * (5.0 / 1023.0);
  float temp = (voltage - 0.5) * 100;

  if(temp < 25){
    digitalWrite(bled, HIGH);
    digitalWrite(gled, LOW);
    digitalWrite(yled, LOW);}
  else if(temp < 35){
    digitalWrite(bled, LOW);
    digitalWrite(gled, HIGH);
    digitalWrite(yled, LOW);}
  else{
    digitalWrite(bled, LOW);
    digitalWrite(gled, LOW);
    digitalWrite(yled, HIGH);}

int lightLevel = analogRead(ldr);
  if(lightLevel < 500){
    digitalWrite(oled, HIGH);}
  else{
    digitalWrite(oled, LOW);}

int fuel = analogRead(fuelsensor);
  if(fuel < 300){
    digitalWrite(rled, HIGH);}
  else{
    digitalWrite(rled, LOW);}

if(digitalRead(irsensor) == HIGH){
    tone(buzzer, 1000);}
else{
    noTone(buzzer);}

if(digitalRead(lock) == LOW){
    door.write(0);
    Serial.println("Door Locked");
    delay(300);}

if(digitalRead(unlock) == LOW){
    door.write(90);
    Serial.println("Door Unlocked");
    delay(300);}

Serial.print("Temp: ");
Serial.print(temp);
Serial.print("  Fuel: ");
Serial.print(fuel); 
Serial.print("  Light: ");
Serial.println(lightLevel);
delay(200);}