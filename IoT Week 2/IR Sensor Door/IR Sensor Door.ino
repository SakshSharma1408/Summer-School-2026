#include<Servo.h>
int led = 13;

void setup()
{ pinMode(led, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(receive);}

void loop(){}

void receive(int bytes)
{int x = Wire.read();
 if(x == 1){
  digitalWrite(led, HIGH);}
 else{
  digitalWrite(led, LOW);}}